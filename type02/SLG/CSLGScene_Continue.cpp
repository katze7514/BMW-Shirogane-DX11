#include "stdafx.h"

#include "../mode.h"

#include "../Hero/IDHero.h"
#include "../BMW/IDBMW.h"

#include "../Scene/IDScene.h"

#include "Map/CMap.h"

#include "Map/CMapChip.h"
#include "Map/CMapChipChara.h"
#include "Map/CMapChipCharaSprite.h"
#include "Map/CMapChipChara2.h"
#include "Map/CMapChipCharaSprite2.h"

#include "Event/CEvent.h"

#include "VM/CSLGFactory.h"
#include "VM/CSubroutineFactorySLG.h"
#include "VM/CSlgVM.h"

#include "Context/CSLGDef.h"
#include "Context/CDataCharaSLG.h"
#include "Context/CMapSymbolDB.h"

#include "IDRule.h"
#include "CSLGScene.h"

namespace BMW{
namespace SLG{

void setContFileName(string& sFile)
{
	sFile.clear();
	sFile=BMW::sSaveFolder + "\\";
	sFile+=sContinue;
}

void CSLGScene::continueSave()
{// コンテニューセーブ
	CSerialize s;

	// 主人公のレベルを反映
	CDataCharaSLG* pChara = context_.getCharaData(context_.getApp()->getExec().getHero()==Hero::Target::HARUNA?"HARUNA":"TAKUMI");
	if(pChara!=NULL) context_.getApp()->getExec().setLv(pChara->getBattle().getLv());

	// セーブデータ
	s << context_.getApp()->getExec();
	// コンテキスト
	s << context_;
	
	string sFile;
	setContFileName(sFile);
	s.Save(sFile);
}

void CSLGScene::continueInit(Task::CTaskContext* pContext)
{// コンテニュー時はこっちだよ
#ifdef STAGE_CREATE
	DEBUG_ADD_SLG_ID_START = DEBUG_ADD_SLG_ID_START_CONST;
#endif

	// まずは、コンテニューデータを保持
	CSerialize s;
	s.SetStoring(false);
	string sFile;
	setContFileName(sFile);
	s.Load(sFile);

	// セーブデータ
	Save::CExecData& save = pContext->getApp()->getExec();
	save.clear();
	s << save;

	// コンテキスト
	s << context_;
	context_.setValue(0,Flag::CONTINUE);
	// ↑これで、不完全な状態でデータが構築されているので、
	// データの完全化
	context_.completeData();

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
	// 対象マップにスクロール
	if(context_.getTargetMap()>=0)
		pMap->scrollIndex(context_.getTargetMap());

	// キャラの追加
	// 味方
	addCharaList(pMap,context_.getPlayerPhaseList());
	// 敵
	addCharaList(pMap,context_.getEnemyPhaseList());
	// 中立
	addCharaList(pMap,context_.getNeutralPhaseList());

	// VMの構築
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
	pVM->setTaskListFactory(smart_ptr<Task::ITaskListFactory>(pFactory));
	// MAINがエントリポイント
	//pVM->jumpTaskList(SLG::Rule::MAIN);
	pVM->jumpTaskList(SLG::Rule::CONTINUE_START);
	// また、VM実行開始前に、FADE_OUTしておくので、一端VMの動きをストップ
	//pVM->valid(false);
	
	// フェードアウト&鳴っていたBGMフェードイン
	//pContext->getApp()->getFoward()->setFaderHandler(fun);
	//pContext->getApp()->getFoward()->fadeOut();
	setState(FADE_C);
	//if(context_.getBgm()>=0)
	//{// BGMが鳴ってたらそれをならす
	//	pContext->getBgmSound()->change(context_.getBgm());
	//	pContext->getBgmSound()->Play();
	//}
}

void CSLGScene::addCharaList(Map::CMap* pMap, list<int>& List)
{
	list<int>::iterator it;
	for(it=List.begin(); it!=List.end(); it++)
		addChara2(*it); 
		//addChara(*it); 
}
/*
void CSLGScene::addChara(int nID)
{
	// 与えられたslgIDを持つキャラを単にマップに追加する
	CDataCharaSLG* pChara = context_.getCharaData(nID);
	if(pChara==NULL) return;

	// マップの取得
	smart_ptr<Map::CMap> pMap = context_.getMap();

	// マップへの追加
	Map::CMapChip* pMapChip = pMap->getMapChip(pChara->getState().getIndex());
	if(pMapChip==NULL) return;

	// マップチップキャラの生成
	Map::CMapChipChara* pChip = new Map::CMapChipChara();
	pChip->setID(pChara->getID());
	pMapChip->addTask(pChip, Map::CMapChip::CHARA);

	// マップチップキャラスプライトの生成
	Map::CMapChipCharaSprite* pSprite = new Map::CMapChipCharaSprite();
	// マップキャラへの追加
	pChip->addTask(pSprite, Map::CMapChipChara::CHARA);
	// 状態の共有
	pSprite->setCharaState(smart_ptr<CCharaState>(pChara->getStatePtr(),false));
	// チップの設定
	smart_ptr<CMapSymbolDB>& pDB = pChara->getMapSymbol();
	pDB->setGraphic(pSprite->getGraphic(0,0),"BEFORE_TOP");
	pDB->setGraphic(pSprite->getGraphic(0,1),"BEFORE_LEFT");
	pDB->setGraphic(pSprite->getGraphic(0,2),"BEFORE_BOTTOM");
	pDB->setGraphic(pSprite->getGraphic(0,3),"BEFORE_RIGHT");
	pDB->setGraphic(pSprite->getGraphic(1,0),"AFTER_TOP");
	pDB->setGraphic(pSprite->getGraphic(1,1),"AFTER_LEFT");
	pDB->setGraphic(pSprite->getGraphic(1,2),"AFTER_BOTTOM");
	pDB->setGraphic(pSprite->getGraphic(1,3),"AFTER_RIGHT");
}
*/

Task::ITaskBase* CSLGScene::addChara2(int nID)
{
	// 与えられたslgIDを持つキャラを単にマップに追加する
	CDataCharaSLG* pChara = context_.getCharaData(nID);
	if(pChara==NULL) return NULL;

	// マップの取得
	smart_ptr<Map::CMap> pMap = context_.getMap();

	// マップへの追加
	Map::CMapChip* pMapChip = pMap->getMapChip(pChara->getIndex());
	if(pMapChip==NULL) return NULL;
	// マップチップキャラの生成
	Map::CMapChipChara2* pChip = new Map::CMapChipChara2();
	// マップチップへの追加
	pMapChip->addTask(pChip, Map::CMapChip::CHARA);
	// マップチップキャラ設定
	pChip->setID(pChara->getID());
	// 状態の共有
	pChip->setCharaState(smart_ptr<CCharaState>(pChara->getStatePtr(),false));

	// マップチップキャラスプライトの生成
	Map::CMapChipCharaSprite2* pSprite = new Map::CMapChipCharaSprite2();
	// マップキャラへの追加
	pChip->addTask(pSprite, Map::CMapChipChara::CHARA);
	// チップの設定
	smart_ptr<CMapSymbolDB>& pDB = pChara->getMapSymbol();
	// マップチップのアニメ展開
	using namespace ChipMovie;
	// 歩き
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_TOP"),WALK_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_LEFT"),WALK_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_BOTTOM"),WALK_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_RIGHT"),WALK_RIGHT);
	// ジャンプ
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP1"),JUMP_READY_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP2"),JUMP_UP_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP3"),JUMP_DOWN_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT1"),JUMP_READY_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT2"),JUMP_UP_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT3"),JUMP_DOWN_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM1"),JUMP_READY_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM2"),JUMP_UP_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM3"),JUMP_DOWN_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT1"),JUMP_READY_RIGHT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT2"),JUMP_UP_RIGHT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT3"),JUMP_DOWN_RIGHT);
	// 静止
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_TOP"),BEFORE_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_LEFT"),BEFORE_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_BOTTOM"),BEFORE_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_RIGHT"),BEFORE_RIGHT);
	// 行動済み静止
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_TOP"),AFTER_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_LEFT"),AFTER_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_BOTTOM"),AFTER_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_RIGHT"),AFTER_RIGHT);
	// 静止ピンチ
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_TOP"),BEFORE_PINCH_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_LEFT"),BEFORE_PINCH_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_BOTTOM"),BEFORE_PINCH_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_RIGHT"),BEFORE_PINCH_RIGHT);
	// 行動済み静止ピンチ
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_TOP"),AFTER_PINCH_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_LEFT"),AFTER_PINCH_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_BOTTOM"),AFTER_PINCH_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_RIGHT"),AFTER_PINCH_RIGHT);

	return pSprite;
}

} // namespace SLG end
} // namespace BMW end