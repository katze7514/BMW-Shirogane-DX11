#include "stdafx.h"

#include "../Scene/IDScene.h"
#include "../Scene/Unit/IDHelp.h"

#include "../SLG/IDSLG.h"

#include "CInterChara.h"
#include "CDataItemInter.h"

#include "CInterTaskListCtrl.h"
#include "CInterFactory.h"
#include "CInterScene.h"

namespace BMW{
namespace Inter{

void CInterScene::OnInit(Task::CTaskContext* pContext)
{
	setContext(pContext);
	setGuiDefDB("INTERMISSION");

	// アイテム設定
	setItem();
	// キャラ設定
	setChara();

	// コントローラ
	CInterTaskListCtrl* pChache = new CInterTaskListCtrl();
	addTask(pChache,CTRL);
	pChache->setTaskListFactory(smart_ptr<Task::ITaskListFactory>(new CInterFactory()));
	// キャッシュ生成
	pChache->createChache(Scene::SELECT,&context_);
	pChache->createChache(Scene::CHARA,&context_);

	// まずはキャラセレクトから
	pChache->jumpTaskList(Scene::SELECT);

	// フェード
	pContext->getBgmSound()->change("STATUS");
	pContext->getBgmSound()->FadeIn(30);

	Draw::CFader::FaderEvent fun;
	fun.set(this,&CInterScene::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(fun);
	pContext->getApp()->getFoward()->fadeOut();
	setState(FADE);
}

void CInterScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END:
		pContext->push(context_.top());
		context_.pop();
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getBgmSound()->FadeOut(30);
		pContext->getApp()->getFoward()->fadeIn(15);
		setState(FADE_END);
	break;

	case DATA:
		// こっちだと、SAVE/LOADモード
		pContext->push(1);
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->callTaskList(BMW::Scene::ID::DATA,true);
		setState(NORMAL);
	break;

	default: break;
	}
}

void CInterScene::OnReset(Task::CTaskContext* pContext)
{
	context_.clearFlag();
	context_.clearItemData();
	context_.clearCharaData();
	setItem();
	setChara();
	// シナリオデータが変わってるかもしれないので設定し直し
	pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pContext->getApp()->getScenario().getScenario(pContext->getApp()->getExec().getStory()),false));
	context_.setScenarioData(pContext->getScenarioData());
	// ここに来ると言うことは、一度は初期化されてるので、
	// フラグを立て直しておく
	context_.init(true);
	static_cast<CInterTaskListCtrl*>(getTask(CTRL))->getCurrentTaskList()->OnReset(&context_);
	setState(FADE);

	Draw::CFader::FaderEvent fun;
	fun.set(this,&CInterScene::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(fun);
	pContext->getApp()->getFoward()->fadeOut();
	pContext->getBgmSound()->change("STATUS");
	pContext->getBgmSound()->FadeIn(30);
}

void CInterScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case BMW::Scene::ID::DATA:
	{
		// スタックトップにロード状態が入ってる
		// ロードされたので、状況を再構築する
		if(pContext->top()==0){ OnReset(pContext); }
		else
		{
			Draw::CFader::FaderEvent fun;
			fun.set(this,&CInterScene::eventFade);
			pContext->getApp()->getFoward()->setFaderHandler(fun); 
		}
		pContext->pop();
	}	
	break;

	default: break;
	}
}

///////////////////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////////////////
void CInterScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		pContext->getInput()->cursolVisible(true);
		pContext->getInput()->guard(false);
		setState(NORMAL);

		// ヘルプモード
		Unit::Help::callHelp(Unit::Help::INTER_SELECT,"INTER_SELECT",pContext);
	}
	ef(getState()==FADE_END)
	{
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->returnTaskList();
	}
}

///////////////////////////////////////////////////////
// データ生成
///////////////////////////////////////////////////////
void CInterScene::setItem()
{// セーブデータからアイテムリストを生成
	Save::CExecData::item_map& mapItem = context_.getApp()->getExec().getItemMap();

	Save::CExecData::item_map::iterator it;
	CDataItemInter* pItem;
	for(it=mapItem.begin(); it!=mapItem.end(); it++)
	{
		pItem = new CDataItemInter();
		pItem->setID(it->first);
		pItem->setNum(it->second);
		context_.setItemData(it->first, pItem);
	}
}

namespace{
__inline bool IsChild(CInterContext& context, int nChild, const string& sParent)
{
	return context.getApp()->getChara().IsChild(nChild,Chara::Const::charaID_.getValue(sParent));
}

__inline void setCharaInterData(CInterChara* pChara, int nCharaID, int& nID, list<int>& valid, const Chara::CCharaDB& charaDB, CInterContext& context)
{
	CInterChara* pChara2 = new CInterChara();
	// コンテキストへ設定
	context.setCharaData(nCharaID, pChara2);
	// アイテムデータ共有
	pChara2->getData()->setItemOffset(pChara->getData()->getItemOffset());
	// データ設定
	charaDB.setInter(pChara2->getData(), nCharaID, *(pChara->getData()->getTrainData()));
	// ID設定
	pChara2->setID(nID++);
	// 有効リストへ
	valid.push_back(nCharaID);
	// ボタンとか作るよ
	pChara2->OnInit(&context);
}

} // namespace end

void CInterScene::setChara()
{// セーブデータからキャラデータを生成
	Save::CExecData& save = context_.getApp()->getExec();

	// とりあえず、VICTORYフラグ設定
	//if(save.getFlag("VICTORY",-1))
	//{// 定義されてなかったら現在の難易度に合わせてフラグセット
	if(context_.getApp()->getScenario().getScenario(save.getNextScenario())->getExpertRank(save.getExpert())==SLG::Expert::HARD)
		save.setFlag("VICTORY",1);	// HARD
	else
		save.setFlag("VICTORY",0);	// NORMAL
	//}

	list<int>& valid = context_.getCharaList();
	// 一応、クリアしておく
	valid.clear();

	// 有効なキャラのIntermission用データ生成
	// アイテムの絡みがあるので、とりあえず、全キャラInterデータを生成する
	Save::CExecData::train_map& Train = save.getTrainMap();
	Save::CExecData::train_map::iterator it;

	int nID=1;
	const Chara::CCharaDB& charaDB = context_.getApp()->getChara();
	CInterChara *pAce=NULL;
	CInterChara*			pChara;
	CDataItemInter*			pItem;
	for(it=Train.begin(); it!=Train.end(); ++it)
	{
	#ifdef BMW_DEBUG
		CDbg().Out("InterChara %d",it->first);
	#endif
		// なぜか-1が入り込むことがあるっぽい
		if(it->first<0) continue;

		// キャラデータを生成して設定
		pChara = new CInterChara();
		context_.setCharaData(it->first, pChara);

		Chara::CDataCharaInter* pInter = pChara->getData();

		// ここでの生成は主になるので、Item補正値を生成
		pChara->getData()->setItemOffset(smart_ptr<BMW::Chara::CStatusItem>(new BMW::Chara::CStatusItem()));
		// データの設定

		// 間違ったデータを補正する
		//if(it->first==Chara::Const::charaID_.getValue("PLAYER_SABER_EX")) it->second->setLv(13);

		charaDB.setInter(pInter, it->first, *it->second);

		// 保持アイテムに従ってアイテムデータを設定する
		Item::CItemDB& db = context_.getApp()->getItem();
		pInter->beginItem();
		while(!pInter->endItem())
		{
			Chara::CStatusAbility& item = *pInter->nextItem();
			pItem = context_.getItemData(item.getID());
			pItem->addChara(pInter->getTrainData()->getID()/*it->first*/);
			// また、アイテムによる能力UP等があればここで行う
			if(db.IsStatus(item.getID()))
				db.applyStatus(*pInter, item.getAttr(), item.getID());
		}

		if(save.IsValid(it->first))
		{// 養成等が有効なキャラだったら、リストに入れる
			pChara->setID(nID++);
			valid.push_back(it->first);

			// 変身キャラだったら、残りも追加
			setMetamor(nID, pChara);
			
			// ACE判定
			if(pAce==NULL
			|| pAce->getData()->getKill() < pChara->getData()->getKill())
				pAce = pChara;
		}

		// ボタンとかの生成
		pChara->OnInit(&context_);
	}

	// ACEを設定
	context_.getApp()->getExec().setAce(pAce->getData()->getID());
}

void CInterScene::setMetamor(int& nID, CInterChara* pChara)
{
	Save::CExecData& save = context_.getApp()->getExec();
	list<int>& valid = context_.getCharaList();
	const Chara::CCharaDB& charaDB = context_.getApp()->getChara();

	// validリストがソートされてることが前提になっているので注意
	if(IsChild(context_,pChara->getData()->getID(),"PLAYER_ARC_FTS"))
	{// ファンタズムーンが来たぞ！
		// エクリプスは有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_ECLIPS")))
		{// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_ECLIPS"), nID, valid, charaDB, context_);
		}
		// アルクェイド
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_ARCUEID")))
		{// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_ARCUEID"), nID, valid, charaDB, context_);
		}
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_RIN_KALEIDO"))
	{// 凛が来たぞ！
		// カレイドルビーは有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_KALEIDO")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_KALEIDO"), nID, valid, charaDB, context_);
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_KOHAKU_2"))
	{// 琥珀2だ。琥珀2を先にしておかないと、IsChildでひっかかってしまう
		// アンバ2は有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_AMBER_2")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_AMBER_2"), nID, valid, charaDB, context_);
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_KOHAKU"))
	{// 琥珀だ
		// アンバは有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_AMBER")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_AMBER"), nID, valid, charaDB, context_);
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_SABER_AVALON"))
	{// セイバー（アヴァロン装備）だ。セイバー（アヴァロン装備）を先にしておかないと、IsChildでひっかかってしまう
		// リリィ（アヴァロン装備）は有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON"), nID, valid, charaDB, context_);
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_SABER_EX"))
	{// セイバー（エスカリ装備）は有効か？
		// リリィ（エスカリ装備）は有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY_EX")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_LILY_EX"), nID, valid, charaDB, context_);
	}
	ef(IsChild(context_,pChara->getData()->getID(),"PLAYER_SABER"))
	{// セイバーだ。セイバーを先にしておかないと、IsChildでひっかかってしまう
		// リリィは有効か？
		if(save.IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY")))
		// 有効だ！　じゃ、生成すんぞ
			setCharaInterData(pChara, Chara::Const::charaID_.getValue("PLAYER_LILY"), nID, valid, charaDB, context_);
	}
}

} // namespace Inter end
} // namespace BMW end