#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/CYesNoUnit.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../../Status/status_fun.h"

#include "../IDRule.h"
#include "../IDSLG.h"
#include "../CSLGScene.h"
#include "../Action/IDAction.h"
#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"
#include "../Event/CEvent.h"
#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CSally_setup_weapon.h"
#include "CSally_view.h"
#include "CSally_add_chara.h"
#include "CSally_select.h"

namespace BMW{
namespace SLG{
namespace Sally{

CSally_select::~CSally_select()
{
	if(pMapChip_==NULL)	DELETE_SAFE(pEv_);
}

////////////////////////////////////////////
// タスク
////////////////////////////////////////////
void CSally_select::OnReset(Task::CTaskContext* pContext)
{
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// 全体を取得
	pPanel_ = db.createInterfaceCast<GUI::CPanel>("PANEL_SALLY_SELECT");
	addTask(pPanel_, WAIT);

	// イベントキャラ
	pEv_ = db.createInterfaceCast<GUI::CGraphic>("EV_G");
	pEv_->setX(32);
	pEv_->setY(-32);

	GUI::CButton::ButtonEvent funButton;
	// 出撃パネル
	GUI::CPanel* pSally = pPanel_->getWidgetCast<GUI::CPanel>("PANEL_SYUTSUGEKI");
	pRemain_ = pSally->getWidgetCast<GUI::INum>("SYUTSUGEKI_SUM");

	// 出撃決定ボタン
	// イベントハンドラ
	funButton.set(this,&CSally_select::eventChara);
	GUI::CButton::setButtonEvent(pSally->getWidgetRecCast<GUI::CButton>("SYUTSUGEKI_B"),funButton,SALLY);

	// ステータス
	pStatus_ = db.createInterfaceCast<GUI::CPanel>("EASY_STATUS1_2");
	pStatus_->setX(34);
	pStatus_->setY(460);
	addTask(pStatus_,STATUS);

	// サークルメニュー
	pMenu_ = db.createInterfaceCast<GUI::CCircleMenu>("SYUTSUGEKI_CIRCLE");
	addTask(pMenu_,MENU);
	// イベントハンドラ
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CSally_select::eventCircle);
	pMenu_->setEventHandler(funCircle);
	// ボタン
	funButton.set(this,&CSally_select::eventButton);
	pMenu_->setButtonEventHandler("WAIT",funButton,PUT_OUT);
	pMenu_->setButtonEventHandler("CHANGE",funButton,CHANGE);
	pMenu_->setButtonEventHandler("SPEC",funButton,SPEC);
	pMenu_->setButtonEventHandler("ARC",funButton,ARC);
	pMenu_->setButtonEventHandler("PHANTAS",funButton,PHANTAS);
	pMenu_->setButtonEventHandler("ECLIPS",funButton,ECLIPS);
	pMenu_->setButtonEventHandler("RIN",funButton,RIN);
	pMenu_->setButtonEventHandler("KALEIDO",funButton,KALEIDO);
	pMenu_->setButtonEventHandler("KOHAKU",funButton,KOHAKU);
	pMenu_->setButtonEventHandler("AMBER",funButton,AMBER);
	pMenu_->setButtonEventHandler("SABER",funButton,SABER);
	pMenu_->setButtonEventHandler("LILY",funButton,LILY);

	// エフェクト
	pIntroEffect_ = static_cast<Movie::CMovieClip*>(db.getSymbolDB().createSymbolStr("INTRO_SYUTSUGEKI"));
	addTask(pIntroEffect_,INTRO_EFFECT);

	p = static_cast<CSLGContext*>(pContext);
}

namespace{

__inline int getCharaID(const string& sID)
{
	return Chara::Const::charaID_.getValue(sID);
}

__inline bool IsLoadMetaChara(int nID, const string& sID, const string& sLoadID, set<int>& setLoadChara, CSLGContext* p)
{
	return nID==getCharaID(sID)
		&& (setLoadChara.find(getCharaID(sLoadID))!=setLoadChara.end()
			|| p->searchSlg(getCharaID(sLoadID))>=0)
		;
}

__inline bool IsMetaCharaLoad(int nID, set<int>& setLoadChara, CSLGContext* p)
{
	// アルク系？
	if(IsLoadMetaChara(nID,"PLAYER_ARC_FTS","PLAYER_ECLIPS",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_ARC_FTS","PLAYER_ARCUEID",setLoadChara,p))
	{// ファンタズムーンだ！
	// エクリプスorアルクがロード済み！ なら、変身対象扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_ECLIPS","PLAYER_ARC_FTS",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_ECLIPS","PLAYER_ARCUEID",setLoadChara,p))
	{// ファンタズムーン・エクリプスだ！
	 // ファンタズムーンorアルクがロード済み！ なら、変身対象扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_ARCUEID","PLAYER_ARC_FTS",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_ARCUEID","PLAYER_ECLIPS",setLoadChara,p))
	{// アルクだ！
	 // ファンタズムーンorエクリプスがロード済み！　なら、変身対象扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_RIN_KALEIDO","PLAYER_KALEIDO",setLoadChara,p))
	{// 凛だ！　カレイドルビーがロード済み！　なら、変身扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_KALEIDO","PLAYER_RIN_KALEIDO",setLoadChara,p))
	{// カレイドルビーだ！　凛がロード済み！　なら、変身扱いなので、次！
		return true;
	}
	// 琥珀系
	ef(IsLoadMetaChara(nID,"PLAYER_KOHAKU","PLAYER_AMBER",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_KOHAKU_2","PLAYER_AMBER_2",setLoadChara,p))
	{// 琥珀だ！　アンバーがロード済み！　なら、変身扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_AMBER","PLAYER_KOHAKU",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_AMBER_2","PLAYER_KOHAKU_2",setLoadChara,p))
	{// アンバーだ！　琥珀がロード済み！　なら、変身扱いなので、次！
		return true;
	}
	// セイバー系
	ef(IsLoadMetaChara(nID,"PLAYER_SABER","PLAYER_LILY",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_SABER_EX","PLAYER_LILY_EX",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_SABER_AVALON","PLAYER_LILY_AVALON",setLoadChara,p))
	{// セイバーだ！　リリィがロード済み！　なら、変身扱いなので、次！
		return true;
	}
	ef(IsLoadMetaChara(nID,"PLAYER_LILY","PLAYER_SABER",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_LILY_EX","PLAYER_SABER_EX",setLoadChara,p)
	|| IsLoadMetaChara(nID,"PLAYER_LILY_AVALON","PLAYER_SABER_AVALON",setLoadChara,p))
	{// リリィだ！　セイバーがロード済み！　なら、変身扱いなので、次！
		return true;
	}

	return false;
}

} // namespace end

void CSally_select::OnInit(Task::CTaskContext* pContext)
{/*
	スタックには

		最大出撃人数
		スクロールINDEX
		Index数分積まれてる
		-1
		除外キャラ数分積まれている
		-1
		イベント数分積まれてる
		-1
 */	
	// 最大出撃人数
	nMaxChara_ = p->top();
	p->pop();

	if(nMaxChara_<0)
	{// 負だったら今マップにいる人数とする
		nMaxChara_ = p->getPlayerPhaseList().size();
	}

	// スクロールINDEX
	int nScroll = p->top();
	p->pop();

	// 有効なIndex
	set<int>& validMap = p->getIndexSet();
	p->clearMove();
	while(p->top()>=0)
	{// 有効なマップインデックスセットに追加
		validMap.insert(p->top());
		p->pop();
	}
	p->pop();

	// 除外キャラ
	CDataCharaSLG* pChara;
	setOut_.clear();
	while(p->top()>=0)
	{
		pChara = p->getCharaData(p->top());
		if(pChara==NULL	|| !pChara->IsExist() || pChara->getIndex()<0){ p->pop(); continue; }
		setOut_.insert(p->top());
		// 除外キャラの場所は自動的に除外になる
		validMap.erase(pChara->getIndex());
		p->pop();
	}
	p->pop();

	// 一端全員AFTER
	p->actAfter(Phase::PLAYER,setOut_);

	// ロード済みキャラセット
	set<int> setLoadChara;
	setCtrlChara_.clear();
	// イベントキャラ
	setEvent_.clear();
	// 一人目のキャラの向きに追加するので、
	// そのためのデータ取得
	pChara = p->getCharaData(p->top());
	if(pChara==NULL) // イベントキャラが一人もいなかったらキャラリスト先頭のを取ってくる
		pChara = p->getCharaData(p->getPlayerPhaseList().front());

	nToward_ = pChara->getState().getWay();

	// ScrollIndexが負だったらイベントキャラの一人目の場所にフォーカス
	if(nScroll<0) nScroll = pChara->getIndex();

	// マップスクロール
	p->getMap()->scrollIndex(nScroll);

	while(p->top()>=0)
	{// イベントキャラセット設定
		pChara = p->getCharaData(p->top());
		if(pChara!=NULL) 
		{
			setEvent_.insert(p->top());
			// 操作可能リストへ
			setCtrlChara_.insert(p->top());
			// ロード済みキャラセットへ
			setLoadChara.insert(p->top());
			// イベントキャラの場所は自動的に有効になる
			validMap.insert(pChara->getIndex());
		}
		p->pop();
	}
	// -1をpop
	p->pop();

	// 除外キャラが取得できたので、操作可能なキャラリストを生成
	// 操作可キャラリストは、セーブデータのvalidSetに入ってる
	// で、まだ、キャラデータがロードされてなかったら、
	// 一度ロードしてしまう
	int nSlg;
	set<int>::iterator it;
	set<int> setOut;
	list<int> listParam;
	Save::CExecData& exec = p->getApp()->getExec();
	exec.beginValid();
	while(!exec.endValid())
	{
		it = exec.nextValid();
		// validSetの中はCharaIDなので
		// 一度、SLGIDへの変換を試みる
		// 変換できた＝データがロードされている
		nSlg = p->searchSlg(*it);
		if(nSlg<0)
		{// データがロードされていないので、ロード
			// 変身用キャラ？
			if(IsMetaCharaLoad(*it,setLoadChara,p)) continue;

			CDataCharaSLG* pChara = new CDataCharaSLG();
			// データ設定
			CSally_add_chara::initCharaData(*it, p->getNextSally(),
											Phase::PLAYER, -1,
											Action::PLAYER, listParam,
											pChara, p);
			// 状態初期設定
			pChara->getState().apper(true);
			pChara->getState().setAct(Act::BEFORE);
			// キャラマップへ追加
			p->setCharaData(p->getNextSally(),pChara,false);
			// マップシンボルロード
			// それまでに、同じシンボルを持ってるやつがいたら共有
			int nID = p->searchMapSymbol(pChara->getBattle().getMapSymbolID(), pChara->getPhase());
			if(nID>=0)
				pChara->setMapSymbol(p->getCharaData(nID)->getMapSymbol());
			else
				pChara->createMapSymbol(pChara->getBattle().getMapSymbolID());

			// ロード時にPhaseStartアクションを引っかけとく
			pChara->actionPhaseStart(*p,true);

			// 外にいるキャラリストへ
			setOut.insert(p->getNextSally());
			// また、操作可能キャラリストへ
			setCtrlChara_.insert(p->getNextSally());
			// ロード済みキャラセットへ
			setLoadChara.insert(*it);
			// NextSallyを一つ進める（INT_MAXからさかのぼってくる）
			p->setNextSally(p->getNextSally()-1);
		}
		ef(setOut_.find(nSlg)==setOut_.end())
		{// 除外リストに入って無かったら、
			// 操作可能リストへ
			setCtrlChara_.insert(nSlg);
			// ロード済みキャラセットへ
			setLoadChara.insert(*it);
		}
	}
	
	// 現在マップ上に居るキャラは、
	// 現在のPlayerPhaseList - 除外キャラ
	setMapChara_.clear();
	// 現在のフェーズリストの必要な部分を保存
	listPlayerPhaseList_.clear();
	list<int> &listPhase = p->getPlayerPhaseList();
	for(list<int>::iterator it=listPhase.begin(); it!=listPhase.end(); ++it)
	{// 操作可能キャラセット・イベントキャラセットに居たら、保存しない
		if(setCtrlChara_.find(*it)==setCtrlChara_.end())
			listPlayerPhaseList_.push_back(*it);
		// 除外キャラに入ってないなら、マップにいる
		if(setOut_.find(*it)==setOut_.end())
			setMapChara_.insert(*it);
	}

	// 保存したので、キャラアウト情報をPlayerPhaseListへ
	listPhase.clear();
	listPhase.insert(listPhase.end(), setOut.begin(), setOut.end());

	// 有効なマップにいろつけ
	Map::CMapChip* pMapChip;
	for(set<int>::iterator it=validMap.begin(); it!=validMap.end(); ++it)
	{
		pMapChip = p->getMapChip(*it);
		if(pMapChip==NULL) continue;
		pMapChip->getMapChipState()->setMove(1);
	}
	// 移動範囲表示
	Map::CMapChipState::move(true);
	// 出撃可能人数
	pRemain_->setNum(nMaxChara_ - (int)setMapChara_.size());

	// ステータス
	pStatus_->visible(false);

	// メニュー
	pMenu_->valid(false);
	pMenu_->visible(false);

	// エフェクト
	pIntroEffect_->OnReset(p);
	pIntroEffect_->valid(true);
	pIntroEffect_->visible(true);

	// パネル全体
	pPanel_->valid(true);

	setState(INTRO);
	nMapSelect_=-1;
	nArc_=-1;
	nRin_=-1;
	nKohaku_=-1;
	nSaber_=-1;
}

void CSally_select::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{// ようは、ここではMAP絡みの処理

	case INTRO:
		if(pIntroEffect_->getTask(0)->getState()==1)
		{// 登場
			setState(CLICK_WAIT);
			pContext->getInput()->guard(false);
		}
	break;

	case CLICK_WAIT:
		if(Input::releaseOK(pContext))
		{// 画面中央に来てWait
			pIntroEffect_->getTask(0)->setState(pIntroEffect_->getTask(0)->getState()+1);
			setState(EXIT);
			pContext->getInput()->guard(true);
		}
	break;
	
	case EXIT:
		if(pIntroEffect_->IsEnd())
		{// 退場。出撃選択通常モードへ
			pIntroEffect_->valid(false);
			pIntroEffect_->visible(false);
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			p->getInput()->guard(false);
			p->getInput()->guardDrag(false);
			p->getInput()->cursolVisible(true);
			Map::CMapChipState::action(true);
			setState(NORMAL);

			// ヘルプモード
			Save::CExecData& save = pContext->getApp()->getExec();
			if(save.getFlag("SLG_CHARA_SELECT2",0) && pRemain_->getNum()>0)
			{// 待機キャラ選べるよ
				save.setFlag("SLG_CHARA_SELECT2",1);
				pContext->getApp()->getFoward()->createHelp(Unit::Help::SLG_CHARA_SELECT2,pContext);
			}
			ef(save.getFlag("SLG_CHARA_SELECT",0))
			{// キャラクター選択って？
				save.setFlag("SLG_CHARA_SELECT",1);
				pContext->getApp()->getFoward()->createHelp(Unit::Help::SLG_CHARA_SELECT,pContext);
			}
		}
	break;

	case NORMAL: // 通常時
	{
		if(p->getTargetMapChip()!=NULL
		&& p->getTargetMapChip()->getMapChipState()->getMove()>0)
		{
			if(Input::releaseOK(pContext))
			{// OK押された
				nTargetMap_ = p->getTargetMap();
				if(p->getTargetChara()>=0)
				{// キャラがいる
					// 選択されたキャラを保存
					nMapSelect_ = p->getTargetChara();
					// 選択したキャラBEFORE
					p->getTargetCharaData()->getState().setAct(Act::BEFORE);
					// サークルメニューを表示
					setMenuIntro();
				}
				else
				{// キャラいねー、出撃キャラに余裕があったら
				 // 出撃キャラ選択へ
					// 本当にキャラがいないなら
					if(p->getTargetMapChip()->getTask(Map::CMapChip::CHARA)==NULL
					&& pRemain_->getNum()>0)
					{// 画面停止
						pPanel_->valid(false);
						p->getInput()->guard(true);
						p->getInput()->guardDrag(true);
						Map::CMapChipState::action(false);
						// キャラ一覧を出撃キャラモードで呼び出し
						pContext->push(CSally_view::SALLY);
						getTaskListCtrl()->callTaskList(Rule::SALLY_VIEW, true);
					}
				}
			}
			ef(Input::releaseCancel(pContext))
			{// キャンセルされた
				if(p->getTargetChara()>=0)
				{// キャラが居たら場所替えのショートカット
					// 押された位置を保存
					nTargetMap_ = p->getTargetMap();
					// 選択されたキャラを保存
					nMapSelect_ = p->getTargetChara();
					// 選択したキャラBEFORE
					p->getTargetCharaData()->getState().setAct(Act::BEFORE);
					// 交換モード
					setState(CHARA);
				}
			}
			else
			{// 何もされてね
				actionStatus(pContext->getValue(Flag::TARGET_CHARA),pContext);
			}
		}
		else
			actionStatus(-1,pContext);
	}
	break;

	case CHARA: // キャラが選択されている
	{
		if(p->getTargetMapChip()!=NULL)
		{
			if(Input::releaseOK(pContext)
			&& p->getTargetMapChip()->getMapChipState()->getMove()>0)
			{// OK押された
				set<int>::iterator it = p->getIndexSet().find(p->getTargetMap());
				if(it!=p->getIndexSet().end()
				&& nTargetMap_!=p->getTargetMap())
				{// 範囲内で、選択位置と同じ場所じゃない
					// 交換
					actionChange(p);
					setState(NORMAL);
				}
			}
			ef(Input::releaseCancel(pContext))
			{// キャンセルされた
				// 元に戻ってメニュー開く
				p->setTargetChara(nMapSelect_);
				p->setTargetMap(nTargetMap_);
				// 改めてメニューを開く
				setMenuIntro();
			}
			else
			{// 何もされてね
				actionStatus(pContext->getValue(Flag::TARGET_CHARA),pContext);
			}
		}
		else
			actionStatus(-1,pContext);
	}
	break;

	case END:
	{
	// 終了処理
	// 現在の状況に合わせて、まずはリストを正常化する
	// 出撃するのは、もちろん、マップ上にいるキャラセットにいるやつら
	// よって、操作可能キャラとマップ上にいるキャラセットの差分のキャラは
	// 必要ないので、削除
	// さらに、出撃選択によってマップに出たキャラに文字列IDを振る
		int n=1;
		for(set<int>::iterator it=setCtrlChara_.begin(); it!=setCtrlChara_.end(); ++it)
		{
			// マップ上にいなかったら削除
			if(setMapChara_.find(*it)==setMapChara_.end())
				p->delCharaData(*it);
			// マップに居て、イベントキャラじゃなければ
			else if(setEvent_.find(*it)==setEvent_.end())
			{// 出撃選択によって出たキャラなので、文字列IDを追加しておく
				p->getSLGDef().setSlgID(Misc::linkStrAndNum("_CHARA",n++),*it);
			}
		}
		// これで、マップが正常化されたので、PlayerPhaseListを構築しなおす
		list<int>& listPhase = p->getPlayerPhaseList();
		listPhase.clear();
		// 元々あったフェーズリストを元に戻す
		listPhase.insert(listPhase.end(), listPlayerPhaseList_.begin(), listPlayerPhaseList_.end());
		// マップに出たやつを追加する
		listPhase.insert(listPhase.end(), setMapChara_.begin(), setMapChara_.end());
		// 変身データストアクリア
		clearMetaorData();
		// マップに出てるやつを対象に武器データを構築する
		set<int>::iterator it_c = setMapChara_.begin();
		// ターゲットキャラに設定
		while(it_c!=setMapChara_.end())
		{
		#ifdef BMW_DEBUG
			CDbg().Out("CharaInit %d",*it_c);
		#endif

			p->setTargetChara(*it_c);
			// 武器をロードしてないなら、武器ロード
			if(!p->getTargetCharaData()->IsWeaponLoad())
				CSally_setup_weapon::setupWeapon(p);

			++it_c;
		}
		
		// 終了処理終了！
		p->getEvent()->validYesNo(false);
		p->actBefore(Phase::PLAYER);
		// 移動範囲非表示
		p->clearMove();
		Map::CMapChipState::move(false);
		// ターゲットキャラとか初期化
		p->setTargetChara(-1);
		p->setCtrlChara(-1);
		p->setTargetMap(-1);
		// リターン
		getTaskListCtrl()->returnTaskList();
	}
	break;

	case CIRCLE: // サークルメニューが開かれている
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// キャンセルされた
			// サークルメニュー閉じる
			p->getTargetCharaData()->getState().setAct(Act::AFTER);
			actionMenu(GUI::CCircleMenu::EXIT,pContext);
			setState(NORMAL);
			p->setTargetChara(-1);
			//nMapSelect_=-1;
		}
	break;

	case YES_CANCEL:
		// YES_NOパネルのキャンセル処理もここで
		// キャンセルされた
		p->getEvent()->validYesNo(false);
		pPanel_->valid(true);
		p->getInput()->guardDrag(false);
		Map::CMapChipState::action(true);
		setState(NORMAL);
	break;

	default: break;
	}
}

void CSally_select::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::SALLY_VIEW)
	{// キャラ一覧から戻ってきた
	 // スタックに選択されたIDが積まれてるはず！
		// 選択されてた！
		// キャラをマップにだすで！
		if(pContext->top()>=0)
			actionInstall(pContext->top(),pContext);
		
		pContext->pop();
		setState(NORMAL);
		pPanel_->valid(true);
		p->getInput()->guard(false);
		p->getInput()->guardDrag(false);
		Map::CMapChipState::action(true);
	}
	ef(nID==Rule::STATUS_RULE)
	{// ステータスから戻ってきた
		pContext->getInput()->guard(false);
	}
}

void CSally_select::setMenuIntro()
{
	p->getMap()->scrollIndex(p->getTargetCharaData()->getIndex());
	// その位置を元にサークルメニュー表示位置計算
	p->setCirclePos(Pos::CHARA_TARGET);
	// メニュー位置
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());
	// キャラメニュー出す
	set<int>::iterator it = setEvent_.find(p->getTargetChara()); 
	// イベントキャラだったら
	if(it!=setEvent_.end())
	{// EV_Gを表示
		pMapChip_ = p->getTargetMapChip();
		pMapChip_->addTask(pEv_,Map::CMapChip::EFFECT);
	}
	// メニューボタン設定
	pMenu_->validButton(true,"CHANGE",p);
	pMenu_->validButton(it==setEvent_.end(),"WAIT",p);
	pMenu_->validButton(true,"SPEC",p);
	// 変身対応キャラかもしれないので、チェック
	int nCharaID = p->getTargetCharaData()->getBattle().getID();
	//CDbg().Out("Menu %d %d",p->getTargetChara(),nCharaID);
	// ファンタズムーンぽくね？
	if(nCharaID==Chara::Const::charaID_.getValue("PLAYER_ARC_FTS"))
	{// なら、エクリプスへ変身できるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ECLIPS")))
			pMenu_->validButton(true,"ECLIPS",p);

		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ARCUEID")))
			pMenu_->validButton(true,"ARC",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_ECLIPS"))
	{// なら、ファンタへ変身できるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ARC_FTS")))
			pMenu_->validButton(true,"PHANTAS",p);

		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ARCUEID")))
			pMenu_->validButton(true,"ARC",p);
	}
	// アルクが選択されてね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_ARCUEID"))
	{// なら、ファンタとエクリプスへ変身できるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ARC_FTS")))
			pMenu_->validButton(true,"PHANTAS",p);
		
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_ECLIPS")))
			pMenu_->validButton(true,"ECLIPS",p);
	}
	// 凛ぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_RIN_KALEIDO"))
	{// なら、カレイドルビーへ転身できるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_KALEIDO")))
			pMenu_->validButton(true,"KALEIDO",p);
	}
	// ルビーぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_KALEIDO"))
	{// なら、凛へ戻れるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_RIN_KALEIDO")))
			pMenu_->validButton(true,"RIN",p);
	}
	// 琥珀ぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_KOHAKU"))
	{// なら、アンバーになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_AMBER")))
			pMenu_->validButton(true,"AMBER",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2"))
	{// なら、アンバーになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_AMBER_2")))
			pMenu_->validButton(true,"AMBER",p);
	}
	// アンバーぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_AMBER"))
	{// なら、琥珀になれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_KOHAKU")))
			pMenu_->validButton(true,"KOHAKU",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_AMBER_2"))
	{// なら、琥珀になれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2")))
			pMenu_->validButton(true,"KOHAKU",p);
	}
	// セイバーぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_SABER"))
	{// なら、リリィになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY")))
			pMenu_->validButton(true,"LILY",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_SABER_EX"))
	{// なら、リリィになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY_EX")))
			pMenu_->validButton(true,"LILY",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_SABER_AVALON"))
	{// なら、リリィになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON")))
			pMenu_->validButton(true,"LILY",p);
	}
	// リリィぽくね？
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_LILY"))
	{// なら、セイバーになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_SABER")))
			pMenu_->validButton(true,"SABER",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_LILY_EX"))
	{// なら、セイバーになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_SABER_EX")))
			pMenu_->validButton(true,"SABER",p);
	}
	ef(nCharaID==Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON"))
	{// なら、セイバーになれるかも
		if(p->getApp()->getExec().IsValid(Chara::Const::charaID_.getValue("PLAYER_SABER_AVALON")))
			pMenu_->validButton(true,"SABER",p);
	}

	actionMenu(GUI::CCircleMenu::INTRO,p);
	p->getInput()->resetInputState();
}

////////////////////////////////////////////
// イベントリスナー
////////////////////////////////////////////

// サークルメニュー用
void CSally_select::eventCircle(int nState, Task::CTaskContext* pContext)
{
	pContext->getInput()->guard(false);
	if(nState==GUI::CCircleMenu::EXIT)
	{// 退場終了
		pPanel_->valid(true);
		pPanel_->visible(true);
		pMenu_->valid(false);
		pMenu_->visible(false);
		p->getInput()->guardDrag(false);
		Map::CMapChipState::action(true);
		if(pMapChip_!=NULL)
		{
			pMapChip_->removeTask(Map::CMapChip::EFFECT);
			pMapChip_=NULL;
		}
	}
		
}

void CSally_select::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{// サークルメニューボタン
	if(GUI::IsRelease(pButton))
	{// サークルボタンが押された
		switch(pButton->getValue())
		{
		case PUT_OUT:// 外す
			// 外す
			actionPutOut(pContext);
			// メニュー閉じる
			actionMenu(GUI::CCircleMenu::EXIT,pContext);
			//nMapSelect_=-1;
			
			setState(NORMAL);
		break;

		case CHANGE:// 交換
			// 交換モードへ
			setState(CHARA);
			// メニュー閉じる
			actionMenu(GUI::CCircleMenu::EXIT,pContext);
		break;

		case SPEC:// ステータス
			pContext->getInput()->guard(true);
			p->setCtrlChara(p->getTargetChara());
			// ステータス開くキャラがまだ武器をロードしてなかったら、
			// ここでロード
			if(!p->getTargetCharaData()->IsWeaponLoad())
			{
				// ロードの際に合体攻撃があるかもしれないので、
				// フェーズリストをマップに居るキャラに変更する
				list<int>& listPhase = p->getPlayerPhaseList();
				// 一時保存
				list<int> listBackup;
				listBackup.insert(listBackup.end(), listPhase.begin(), listPhase.end());
				// 一端クリア
				listPhase.clear();
				// 元々あったフェーズリストを元に戻す
				listPhase.insert(listPhase.end(), listPlayerPhaseList_.begin(), listPlayerPhaseList_.end());
				// マップに出たやつを追加する
				listPhase.insert(listPhase.end(), setMapChara_.begin(), setMapChara_.end());
		
				// 武器セットアップ
				CSally_setup_weapon::setupWeapon(p);

				// ロードが終わったのでフェーズリストを元に戻す
				listPhase.clear();
				listPhase.insert(listPhase.end(), listBackup.begin(), listBackup.end());
			}

			getTaskListCtrl()->callTaskList(Rule::STATUS_RULE,true);
		break;

		default:
		// 変身だ！
			actionMetamor(pButton->getValue());
			// メニュー閉じる
			actionMenu(GUI::CCircleMenu::EXIT,pContext);
			setState(NORMAL);
		break;
		}
	}
}

void CSally_select::eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{// 待機パネル系処理
	if(GUI::IsRelease(pButton))
	{// ボタンが押された
		if(pButton->getValue()==SALLY
		&& pRemain_->getNum()>=0 // maxより多かったらダメ
		&& pRemain_->getNum()<nMaxChara_) // 最大出撃人数だったら、マップにだれもいない
		{// 出撃ボタン押された
		 // 確認ダイアログ出す
			p->getEvent()->validYesNo(true);
			GUI::CButton::ButtonEvent fun;
			fun.set(this,&CSally_select::eventYes);
			Unit::CYesNoUnit& unit = p->getEvent()->getYesNoUnit();
			// 座標設定
			unit.setX(320);
			unit.setY(280);
			// イベントハンドラ
			unit.setButtonHandler(fun,YES,NO);
			Unit::CYesNoUnit::CancelEvent funC;
			funC.set(this,&CSally_select::eventCancel);
			unit.setCancelHandler(funC);
			// 設定ー
			unit.setIntro(pRemain_->getNum()<=0 ? Unit::CYesNoUnit::SALLY : Unit::CYesNoUnit::SALLY2, pContext, pRemain_->getNum());
			
			// 通常のパネルの動作を停止
			pPanel_->valid(false);
			pStatus_->visible(false);
			p->getInput()->guardDrag(true);
			Map::CMapChipState::action(false);
		}
	}
	ef(GUI::IsOverIn(pButton))
	{// ボタンがオーバーイン
		if(pButton->getValue()==SALLY)
		{// 出撃ボタン上
		 // じゃ、一端、いろいろと無効か
			pContext->setValue(-1,Flag::TARGET_MAP);
			actionStatus(-1,pContext);
		}
	}
}

// 確認ダイアログ用
void CSally_select::eventYes(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		if(pButton->getValue()==YES)
		{// YESボタン
			pContext->getInput()->guard(true);
			p->getInput()->guardDrag(true);
			pContext->getInput()->cursolVisible(false);
			Map::CMapChipState::action(false);
			// 後処理
			setState(END);
		}
		ef(pButton->getValue()==NO)
		{// NO
			setState(YES_CANCEL);
		}
	}
}

void CSally_select::eventCancel(Task::CTaskContext* pContext)
{
	// キャンセルされた
	setState(YES_CANCEL);
}

//////////////////////////////////////////////
// アクション
//////////////////////////////////////////////
void CSally_select::actionMenu(int nState, Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::INTRO)
	{
		pPanel_->valid(false);
		p->getInput()->guardDrag(true);
		Map::CMapChipState::action(false);
		setState(CIRCLE);
	}
	pMenu_->valid(true);
	pMenu_->visible(true);
	BMW::Event::IListenerCircleMenu::actionMenu(nState,pContext);
}

void CSally_select::actionInstall(int nID, Task::CTaskContext* pContext)
{// キャラを出現させる
	actionStatus(-1,pContext);
	// 追加方向設定
	CDataCharaSLG* pChara = p->getCharaData(nID);
	pChara->getState().setWay(nToward_);
	// 投入Index設定
	pChara->setIndex(nTargetMap_);
	// マップへ
	smart_ptr<CSLGScene> pSlgScene = smart_ptr_static_cast<CSLGScene>(p->getScene());
	pSlgScene->addChara2(pChara->getID());
	// 出てくる時は、AFTER
	pChara->getState().setAct(Act::AFTER);
	// あと何人を減らす
	pRemain_->setNum(pRemain_->getNum()-1);
	// 待機キャラリストから外す
	p->delPhase(nID,Phase::PLAYER);
	// マップいるリストへ追加
	setMapChara_.insert(nID);
}

void CSally_select::actionPutOut(Task::CTaskContext* pContext)
{// 外す
// マップからキャラを外す
	actionStatus(-1,pContext);
	// キャラデータ取得
	CDataCharaSLG* pChara = p->getCharaData(nMapSelect_);
	// マップから削除
	Map::CMapChip* pMap = p->getMapChip(pChara->getIndex());
	pMap->killTask(Map::CMapChip::CHARA);
	// マップにいますよリストから削除
	setMapChara_.erase(nMapSelect_);
	// IDとキャラデータを一待機データへ
	p->getPlayerPhaseList().push_back(nMapSelect_);
	// あと何人を増やす
	pRemain_->setNum(pRemain_->getNum()+1);
	// ターゲットキャラ
	p->setTargetChara(-1);
}

void CSally_select::actionChange(Task::CTaskContext* pContext)
{// キャラ位置交換
	actionStatus(-1,pContext);
	CDataCharaSLG* pChara;
	// マップ上のキャラ、もしくはマス対象
	// 入れ替え
	pChara = p->getCharaData(nMapSelect_);
	Map::CMapChip* pMapChip = p->getMapChip(pChara->getIndex());
	Task::ITaskBase* pChip = pMapChip->removeTask(Map::CMapChip::CHARA);
	// 対象マップ
	Map::CMapChip* pMapChip2 = p->getTargetMapChip();
	if(pMapChip2->getTask(Map::CMapChip::CHARA)!=NULL)
	{// キャラいたら、外す
		Task::ITaskBase* pChip2 = pMapChip2->removeTask(Map::CMapChip::CHARA);
		// そして、交換対象がいた所に追加
		pMapChip->addTask(pChip2, Map::CMapChip::CHARA);
		// 投入Index設定
		CDataCharaSLG* pChara2 = p->getTargetCharaData();
		pChara2->setIndex(pMapChip->getIndex());
	}
	// 交換
	pMapChip2->addTask(pChip, Map::CMapChip::CHARA);
	// 投入Index設定
	pChara->setIndex(pMapChip2->getIndex());
	pChara->getState().setAct(Act::AFTER);
	p->setTargetChara(nMapSelect_);
}

CDataCharaSLG* CSally_select::getMetaorData(int nCharaID)
{
	map<int, CDataCharaSLG*>::iterator it = mapMetaor_.find(nCharaID);
	if(it!=mapMetaor_.end()) return it->second;
	return NULL;
}

void CSally_select::setMetaorData(int nCharaID, CDataCharaSLG* pChara)
{
	mapMetaor_.insert(pair<int,CDataCharaSLG*>(nCharaID,pChara));
}

void CSally_select::actionMetamor(int nMeta)
{// 変身
	// データ取得
	// 現在のデータを取得
	CDataCharaSLG* pTarget = p->getCharaData(nMapSelect_);

	// nMetaからキャラIDへ変更
	int nCharaID=-1;
	switch(nMeta)
	{
	case PHANTAS: // ファンタズムーンへ
		nArc_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_ARC_FTS");
	break;

	case ECLIPS: // ファンタズムーン・エクリプスへ
		nArc_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_ECLIPS");
	break;

	case ARC: // アルクだってさ
		nArc_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_ARCUEID");
	break;

	case RIN: // 凛へ
		nRin_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_RIN_KALEIDO");
	break;

	case KALEIDO: // カレイドルビーへ
		nRin_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_KALEIDO");
	break;

	case KOHAKU: // 琥珀へ
		if(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_AMBER"))
			nKohaku_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_KOHAKU");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_AMBER_2"))
			nKohaku_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2");
	break;

	case AMBER: // アンバーへ
		if(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_KOHAKU"))
			nKohaku_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_AMBER");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2"))
			nKohaku_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_AMBER_2");
	break;

	case SABER: // セイバーへ
		if(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_LILY"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_SABER");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_LILY_EX"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_SABER_EX");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_SABER_AVALON");
	break;

	case LILY: // リリィへ
		if(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_SABER"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_LILY");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_SABER_EX"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_LILY_EX");
		ef(pTarget->getCharaID()==Chara::Const::charaID_.getValue("PLAYER_SABER_AVALON"))
			nSaber_=nCharaID=Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON");
	break;

	default: // それ以外はありえないので終了
	return;
	}

	if(nCharaID<0) return;

	// まだ、変身マップに入ってなかったら投入
	if(getMetaorData(pTarget->getBattle().getID())==NULL)
		setMetaorData(pTarget->getBattle().getID(),pTarget);

	// 変身後のデータを取得
	CDataCharaSLG* pChara = getMetaorData(nCharaID);
	list<int> listParam;
	if(pChara==NULL)
	{// データねぇ、じゃ、作るか・・・
	#ifdef BMW_DEBUG
		CDbg().Out("METAMOR %d %d", nCharaID, nMapSelect_);
	#endif

		pChara = new CDataCharaSLG();
		// データ設定
		CSally_add_chara::initCharaData(nCharaID, pTarget->getID(),
										Phase::PLAYER, -1,
										Action::PLAYER, listParam,
										pChara, p);
		// それまでに、同じシンボルを持ってるやつがいたら共有
		int nID = p->searchMapSymbol(pChara->getBattle().getMapSymbolID(), pChara->getPhase());
		if(nID>=0)
			pChara->setMapSymbol(p->getCharaData(nID)->getMapSymbol());
		else
			pChara->createMapSymbol(pChara->getBattle().getMapSymbolID());
		// 差分データを適用
		// 説得
		pChara->setPersList(pTarget->getPersList());
		// 状態
		pChara->setState(pTarget->getState());
		// 戦闘データ
		Chara::CDataCharaBattle& target = pTarget->getBattle();
		Chara::CDataCharaBattle& chara = pChara->getBattle();
		// 補正値
		chara.setFundOffset(target.getFundOffset());
		chara.setBattleOffset(target.getBattleOffset());
		// 気力
		chara.setMental(target.getMental());
		// 援護
		chara.setBackUpAttack(target.getBackUpAttack());
		chara.setBackUpDefence(target.getBackUpDefence());
	}
	// 他必要データのコピー
	pChara->getState().setWay(pTarget->getState().getWay());
	// 登場するのでリセット付きPhaseStartアクションを引っかけとく
	// 二重の気力アップや援護のを避けるため
	pChara->actionPhaseStart(*p,true,true);

	// マップから削除
	Map::CMapChip* pMap = p->getMapChip(pTarget->getIndex());
	pMap->killTask(Map::CMapChip::CHARA);
	// キャラマップからの現在データの削除
	p->delMapCharaData(pTarget->getID(),false);
	
	// 今のデータの追加
	p->setCharaData(pChara->getID(),pChara,false);
	// 投入Index設定
	pChara->setIndex(nTargetMap_);
	// マップへ
	smart_ptr<CSLGScene> pSlgScene = smart_ptr_static_cast<CSLGScene>(p->getScene());
	pSlgScene->addChara2(pChara->getID());
	// 出てくる時は、ARFTER
	pChara->getState().setAct(Act::AFTER);
}

void CSally_select::clearMetaorData()
{// 不要になった変身データを削除する
	map<int, CDataCharaSLG*>::iterator it;
	for(it=mapMetaor_.begin(); it!=mapMetaor_.end(); ++it)
	{// リストをまわすで
		if(it->first==nArc_ || it->first==nRin_ || it->first==nKohaku_ || it->first==nSaber_) // 今、出てるやつはスキップ
			continue;

		// そうじゃなかったら、削除
		// 武器がロード済みだったら、削除
		if((it->second)->IsWeaponLoad())
			p->delCharaWeaponData(it->second);
		// キャラデータ削除
		DELETE_SAFE(it->second);
	}
	mapMetaor_.clear();
}

void CSally_select::actionStatus(int nTarget, Task::CTaskContext* pContext)
{
	if(nTarget>=0)
	{// キャラがいたら、ステータス表示
		CDataCharaSLG* pChara = p->getCharaData(nTarget);
		Status::setEasyStatus(pStatus_,*pChara,*p);
		pStatus_->visible(true);
	}
	else if(pStatus_->IsVisible())
	{// いないなら
		pStatus_->visible(false);
	}
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end