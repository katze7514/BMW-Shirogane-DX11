#include "stdafx.h"

#include "../mode.h" 

#ifdef STAGE_CREATE
#include "Sally/CSally_add_chara.h"
#include "Context/CDataCharaSLG.h"
#endif // STAGE_CREATE

#ifdef CHARA_CTRL
#include "Context/CDataCharaSLG.h"
#endif

#include "../BMW/IDBMW.h"

#include "../Scene/IDScene.h"

#include "Map/CMap.h"
#include "Map/CMapChip.h"

#include "Event/CEvent.h"

#include "VM/CSLGFactory.h"
#include "VM/CSubroutineFactorySLG.h"
#include "VM/CSlgVM.h"

#include "Phase/CPhaseBall.h"

#include "Context/CSLGDef.h"

#include "Attack/CAttack_calc.h"

#include "IDRule.h"
#include "CSLGScene.h"

namespace BMW{
namespace SLG{

void CSLGScene::OnInit(Task::CTaskContext* pContext)
{
	// クイック動作のためのインプットデータ
	pInput_ = static_cast<Input::CTaskInput*>(pContext->getInput());
	setState(NORMAL);
	// コンテキストのスタックトップの値が、
	// SLG ID・コンテニューフラグと積まれてる
	context_.setValue(pContext->top(),Flag::ID);
	pContext->pop();
	if(context_.getValue(Flag::ID)>=0)
	{// コンテニューの時は、状況はフラグにセーブされてるので関係ない
		// 全滅プレイをすると熟練度無し
		context_.setValue(pContext->top(),Flag::EXPERT_C);
		// 全滅プレイ
		if(pContext->top()<0)
			context_.setValue(1,Flag::WIPEOUT);

		pContext->pop();
	}

	// イベントハンドラ
	fun.set(this,&CSLGScene::eventFade);

	// コンテキストの設定
	setContext(pContext);
	context_.setBattleData(pContext->getBattleData());

	// 全SLG共通エフェクトDB
	context_.getEffectDB().setSymbol(Config::Const::configDB_.getConfigFileStr("MAP_EFFECT"));

	// IDによって初期化メソッドを振り分ける
	// つまり、コンテニュー復帰の時は、
	// データ生成順序をきいつけないといけないわけで
	if(context_.getValue(Flag::ID)<0)	continueInit(pContext);
	else								newInit(pContext);

	context_.getEvent()->getTurnBall().update(&context_);

	// マップStateのstaticグラフィックデータの設定
	context_.createMapChip();

	// 命中抽選器初期化
	Attack::CAttack_calc::randLot_.init();
}

#ifdef STAGE_CREATE
int CSLGScene::DEBUG_ADD_SLG_ID_START_CONST = 100;
#endif

void CSLGScene::newInit(Task::CTaskContext* pContext)
{// 新規にSLGを生成する
	context_.setValue(1,Flag::CONTINUE);
	// SLG定義スクリプトの読み込み
#ifdef STAGE_CREATE
	CDbg().Out(pContext->getScenarioData()->getScenarioFile(context_.getValue(Flag::ID)));

	DEBUG_ADD_SLG_ID_START = DEBUG_ADD_SLG_ID_START_CONST;
	context_.setValue(DEBUG_ADD_SLG_ID_START_CONST,Flag::ADD_SLG_ID);
#endif
	context_.getSLGDef().setSLGDef(pContext->getScenarioData()->getScenarioFile(context_.getValue(Flag::ID)),&context_);

	// GUI読み込み
	// 勝利条件読み込み関係のためSLGスクリプトを読み込んだあとになる
	setGuiDefDB("SLG");

	// Event
	Event::CEvent* pEvent = new Event::CEvent();
	addTask(pEvent,EVENT);
	pEvent->OnInit(&context_);
	context_.setEvent(smart_ptr<Event::CEvent>(pEvent,false));

	// Map
	Map::CMap* pMap = new Map::CMap(context_.getSLGDef().getMap());
	addTask(pMap,MAP);
	context_.setMap(smart_ptr<Map::CMap>(pMap,false));

	// VM
	CSlgVM* pVM = new CSlgVM();
	addTask(pVM,VM);
	context_.setVM(smart_ptr<CSlgVM>(pVM,false));

	// APIファクトリの取得
	CSubroutineFactorySLG* pFactory = CSLGFactory().createFactory(context_.getSLGDef().getFactory());
	// APIファクトリの設定
	pFactory->setSlgDef(smart_ptr<CSLGDef>(context_.getSLGDefPtr(),false));
	pFactory->OnInit(&context_);
	pFactory->setScript(&context_);
	pFactory->clearFlag(&context_);
	pVM->setTaskListFactory(smart_ptr<Task::ITaskListFactory>(pFactory));
	// MAINがエントリポイント
	pVM->jumpTaskList(SLG::Rule::MAIN);

	// 各種フラグ設定
	context_.setBP(pContext->getApp()->getExec().getBP());
	context_.setFP(pContext->getApp()->getExec().getFP());
	context_.setExpert(pContext->getApp()->getExec().getExpert());
}

void CSLGScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END:
	{	 // SLGのコンテキストのトップは勝利ID
		 // それとSLGシーンIDを戻り値として、積んでおく
		pContext->push(context_.top());
		//CDbg().Out("END %d",context_.top());
		context_.pop();
		pContext->push(context_.getID());

		pContext->getApp()->getSeDB().StopAll();
		pContext->getBgmSound()->FadeOut(30);

		setState(FADE_END);

		Scene::CFoward* pFoward = pContext->getApp()->getFoward();
		if(pFoward->IsFadeEnd() && pFoward->getFadeType()==Draw::CFader::FADE_IN)
		{// すでにフェード済みだったらそのままスルーする
			eventFade(pContext);
		}
		else
		{
			pFoward->setFaderHandler(fun);
			pFoward->setFadeColor(RGB(0,0,0));
			pFoward->fadeIn();
		}
	}
	break;

	case DEMO:
	{
		setState(FADE);
		Scene::CFoward* pFoward = pContext->getApp()->getFoward();
		if(pFoward->IsFadeEnd() && pFoward->getFadeType()==Draw::CFader::FADE_IN)
		{// すでにフェード済みだったらそのままスルーする
			eventFade(pContext);
		}
		else
		{
			pFoward->setFaderHandler(fun);
			pFoward->setFadeColor(RGB(0,0,0));
			pFoward->fadeIn();
		}
	}
	break;

	case NORMAL:
		if(context_.IsQuickLoad()
		&& pInput_->getKeyBoard().IsKeyPush(DIK_L))
		{// クイックロード
			context_.push(SLG::Victory::LOSE);
			context_.setID(-1);

			setState(END);
		}
	#ifdef BMW_DEBUG // デバッグモード時のみ最初から
		else
		{// 長くなって来たのでメソッド化
			slgDebugCmd();
		}
	#endif

	break;

	default: break;
	}
}

void CSLGScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Scene::ID::DEMO)
	{// デモシーンから戻ってきた
		pContext->getApp()->getFoward()->setFaderHandler(fun);
		pContext->getApp()->getFoward()->fadeOut();
		setState(DEMO_FADE);
	}
}

void CSLGScene::eventFade(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case FADE_END:
		pContext->getApp()->getFoward()->clearPopUp();
		pContext->getInput()->guardDrag(true);
		getTaskListCtrl()->returnTaskList();

		setState(NORMAL);
	break;

	case FADE:
		// フェードが終わったら
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->callTaskList(Scene::ID::DEMO,true);

		setState(NORMAL);
	break;

	case DEMO_FADE:
		// フェードが終わったら
		setState(DEMO_END);
		pContext->getInput()->guard(false);
	break;

	case FADE_C:
		// VM動作開始
		getTask(VM)->valid(true);
		setState(NORMAL);
	break;

	default: setState(NORMAL); break;
	}
}

//////////////////////////////////////////
// 以下デバグ系
#ifdef BMW_DEBUG

#ifdef STAGE_CREATE
namespace{

const char* getWayText(int nWay)
{
	switch(nWay)
	{
	case Way::LEFT: return "LEFT";
	case Way::TOP: return "TOP";
	case Way::RIGHT: return "RIGHT";
	default: return "BOTTOM";
	}
}
}
#endif // STAGE_CREATE

void CSLGScene::slgDebugCmd()
{// SLGのOnActionで呼ばれている
	if(pInput_->getKeyBoard().IsKeyPush(DIK_BACK))
	{// 最初から
		context_.push(SLG::Victory::RESTART);
		setState(END);
	}
#ifdef STAGE_CREATE
	// マップ構築のための補助用
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_A))
	{// 対象のマップにキャラを追加
		if(context_.getTargetCharaData()==NULL)
		{// マップにすでに誰かいたら追加しない
			list<int> listParam;
			CDataCharaSLG* pChara = new CDataCharaSLG();
			// SLGデータ設定
			Sally::CSally_add_chara::initCharaData(Chara::Const::charaID_.getValue("ENEMY_MECHHISUI_WEAK"), context_.getValue(Flag::ADD_SLG_ID), Phase::PLAYER, 0, 0, listParam, pChara, &context_);
			// CSLGContextへの追加
			context_.setCharaData(context_.getValue(Flag::ADD_SLG_ID), pChara);
			// フェーズは、↑で自動的に設定される
			// マップチップ生成
			pChara->createMapSymbol(pChara->getBattle().getMapSymbolID());
			// MAP追加のための設定
			pChara->setIndex(context_.getTargetMap());
			pChara->getState().setWay(Way::LEFT);

			addChara2(context_.getValue(Flag::ADD_SLG_ID));
			context_.setValue(context_.getValue(Flag::ADD_SLG_ID)+1,Flag::ADD_SLG_ID);
		}
	}
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_D))
	{// 対象のマップのキャラを削除
		CDataCharaSLG* pData = context_.getTargetCharaData();
		if(pData!=NULL)
		{	
			context_.getMapChip(pData->getIndex())->killTask(Map::CMapChip::CHARA);
			pData->setIndex(-1);
			context_.setValue(-1,Flag::TARGET_CHARA);
			context_.setValue(-1,Flag::TARGET_MAP);
			context_.delPhase(pData->getID(),pData->getPhase());
			context_.delCharaData(pData->getID());
		}
	}
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_S))
	{// 追加したキャラをファイルに出力する
		for(int i=DEBUG_ADD_SLG_ID_START; i<context_.getValue(Flag::ADD_SLG_ID); ++i)
		{
			CDataCharaSLG* pChara = context_.getCharaData(i);
			if(pChara!=NULL
			&& pChara->getIndex()>=0)
			{
				Err.Out("<addchara no=\"%d\" train=\"0\" chara=\"ENEMY_MECHHISUI_WEAK\" phase=\"ENEMY\" action=\"NORMAL_PARAM\" wait=\"0\" move=\"MOVE\" slg=\"CHANGE\" />",pChara->getID());
				Err.Out("<change_chara no=\"%d\" chara=\"ENEMY_MECHHISUI_WEAK\" train=\"3\" flag=\"AVERAGE\" offset=\"1\" />",pChara->getID());
				Err.Out("<setweapon no=\"%d\" />",pChara->getID());
				Err.Out("<addmap no=\"%d\" index=\"%d\" way=\"%s\" />",pChara->getID(),pChara->getIndex(),getWayText(pChara->getState().getWay()));

				Err.Out("");
			}
		}
	}
#endif // STAGE_CREATE
#ifdef CHARA_CTRL
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_B))
	{
		CDataCharaSLG* pData = context_.getTargetCharaData();
		if(pData!=NULL)
		{// 対象のキャラをBEFOREにする
			pData->getState().setAct(Act::BEFORE);
		}
	}
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_P))
	{
		CDataCharaSLG* pData = context_.getTargetCharaData();
		if(pData!=NULL)
		{// 対象のキャラのPINCHをswapする
			pData->getState().pinch(!pData->getState().IsPinch());
		}
	}
	ef(pInput_->getKeyBoard().IsKeyPush(DIK_W))
	{
		CDataCharaSLG* pData = context_.getTargetCharaData();
		if(pData!=NULL)
		{// 対象のキャラの方向を変える
			int nWay = pData->getState().getWay();
			switch(nWay)
			{// 時計回り
			case Way::TOP: nWay=Way::LEFT; break;
			case Way::LEFT: nWay=Way::BOTTOM; break;
			case Way::BOTTOM: nWay=Way::RIGHT; break;
			default: nWay=Way::TOP; break;
			}
			pData->getState().setWay(nWay);
		}
	}

#endif // CHARA_CTRL
}
#endif // BMW_DEBUG

} // namespace SLG end
} // namespace BMW end