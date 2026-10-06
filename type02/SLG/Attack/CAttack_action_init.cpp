#include "stdafx.h"

#include "../../Chara/CValidSpirit.h"
#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../Scene/GUI/CNumRemain.h"
#include "../../Scene/GUI/CNumCtrl.h"
#include "../../Scene/GUI/CGage.h"
#include "../../Scene/GUI/CGraphicFace.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDSLG.h"
#include "../GUI/CStatusCharaVeryEasy.h"
#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"
#include "../Action/IAction.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
//#include "../Map/CMapChipChara2.h"

#include "../slg_fun.h"

#include "CAttack_range2.h"
#include "CAttack_calc.h"
#include "CAttack_road.h"
#include "CAttack_road2.h"

#include "CAttack_action.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_action::OnInit(Task::CTaskContext* pContext)
{// ここまでの状況に合わせて、行動表の中身を構築する
	setState(NORMAL);
	pContext->getInput()->guardCursol(false);
	pContext->getInput()->guard(false);
	pContext->getInput()->cursolVisible(true);

	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 状態のクリア
	state_.clearData();

	// 各種状態の設定
	// 攻撃側キャラデータ
	CDataCharaSLG*	attack = p->getCtrlCharaData();
	state_.setCharaData(attack,CBattleState::ATTACK);
	
	// 反撃側キャラデータ
	CDataCharaSLG*	counter = p->getTargetCharaData();
	state_.setCharaData(counter,CBattleState::COUNTER);

	// 距離の設定
	Map::CMapChip* pCounterMap = p->getMapChip(counter->getIndex());
	nDist_ = pCounterMap->getMapChipState()->getAttack();
	nRealDist_ = pCounterMap->getMapChipState()->getRealDist();
	// 高さの設定
	nHeight_ = pCounterMap->getMapChipState()->getAtkHeight();

	// 攻撃側の方が低かったら、負にする
	if(p->getMapChip(attack->getIndex())->getMapInfo().getHeight() < pCounterMap->getMapInfo().getHeight())
		nHeight_ = -nHeight_;

	// 反撃側は+-が逆になる
	nCounterHeight_ = -nHeight_;

	// 反撃側からの距離
	CAttack_road2 road2;
	road2.setSLGContext(p);
	road2.actionAbility(counter);
	// 移動範囲のクリア
	p->clearMove();

	road2.calcAttack(p->getMapChip(counter->getIndex()),0,0,0,Way::NO);
	// 計算が終わると相手側のMove値として取得できる
	Map::CMapChip* pAttackChip = p->getMapChip(attack->getIndex());
	nCounterDist_= pAttackChip==NULL ? -1 : pAttackChip->getMapChipState()->getMove();
	// 移動範囲のクリア
	p->clearMove();

	// 仲間が対象の武器？
	// ステータス変化武器が、仲間対象
	bool bFriend = p->getCtrlWeaponData()->getKind()==Weapon::Kind::STATUS;

	// 攻撃側命中設定
	state_.setWeaponID(p->getCtrlWeapon(),CBattleState::ATTACK);
	calcAndSetHit(CBattleState::ATTACK, CBattleState::COUNTER, p, bFriend);
	
	// 反撃側命中設定
	CAttack_range2::calcAllRange(counter, p);
	// 仲間対象だったら反撃しない
	int nCounterAction = bFriend ? Battle::HIT : counter->actionCounter(nCounterDist_,nRealDist_,abs(nCounterHeight_),*p,attack->getBattle().getHP());
	state_.setWeaponID(nCounterAction,CBattleState::COUNTER);
	calcAndSetHit(CBattleState::COUNTER, CBattleState::ATTACK, p);
	bImCounter_ = nCounterAction==Battle::HIT;

	// インターフェイスへの反映
	if(p->getPhase()==Phase::PLAYER)
	{// 味方側からの攻撃
		// 攻撃側がNPCキャラ？
		bNpc_ = attack->getAction()->IsNonPlayer();
		// ただし、NPCキャラだったらキャンセルできない
		pCancel_->valid(!bNpc_);
		pCancel_->setValue(CANCEL);
		if(!bFriend)
		{// 援護設定
			// 敵側の援護判定
			resetBackUp();
			calcBackDef(*counter,*p);
			int nCharaID = selectBackUpDef(*p);
			if(nCharaID>=0) // 援護キャラが居れば設定
				state_.setCharaData(p->getCharaData(pairBackUp_[nCharaID].first),CBattleState::COUNTER_BACK);
	
			// 味方側の援護判定
			resetBackUp();
			calcBackAttack(*attack,*p,counter->getBattle().getHP());
			nBackUpPos_=selectBackUpAtk(*p);
			if(nBackUpPos_>=0)// 援護キャラが居れば設定
				state_.setCharaData(p->getCharaData(pairBackUp_[nBackUpPos_].first),CBattleState::ATTACK_BACK);
			
			//setStateBackUpAtk(nBackUpPos_,*p);
		}
		if(pSupport_ != pPlayer_)
		{// 必要があればパネルの入れ替え
			pSupport_ = pPlayer_;
			pSupportR_ = pPlayer_->getWidgetCast<GUI::CPanel>("PANEL_SUPPORT_R");
			removeTask(SUPPORT);
			addTask(pSupport_,SUPPORT);
		}

		// ここまでで全状況が計算されているはず
		initPanel(CBattleState::COUNTER, CBattleState::ATTACK, false, *p);
	}
	else
	{// 敵側からの攻撃
		// 防御側がNPCキャラ？
		bNpc_ = counter->getAction()->IsNonPlayer();
		pCancel_->valid(false);

		if(!bFriend)
		{// 援護設定
			// 敵側の援護判定
			resetBackUp();
			calcBackAttack(*attack,*p,counter->getBattle().getHP());
			int nCharaID=selectBackUpAtk(*p);
			if(nCharaID>=0)
			{// 援護キャラが居れば設定
				state_.setCharaData(p->getCharaData(pairBackUp_[nCharaID].first),CBattleState::ATTACK_BACK);
				state_.setWeaponID(pairBackUp_[nCharaID].second,CBattleState::ATTACK_BACK);
			}
			//setStateBackUpAtk(selectBackUpAtk(*p),*p);
			
			// 味方側の援護判定
			resetBackUp();
			calcBackDef(*counter,*p);
			nBackUpPos_=selectBackUpDef(*p);
			if(nBackUpPos_>=0) // 援護キャラが居れば設定
				state_.setCharaData(p->getCharaData(pairBackUp_[nBackUpPos_].first),CBattleState::COUNTER_BACK);
		}
		if(pSupport_ != pEnemy_)
		{// 必要があればパネルの入れ替え
			pSupport_ = pEnemy_;
			pSupportR_ = pEnemy_->getWidgetCast<GUI::CPanel>("PANEL_SUPPORT_R");
			removeTask(SUPPORT);
			addTask(pSupport_,SUPPORT);
		}
		// ここまでで全状況が計算されているはず
		initPanel(CBattleState::ATTACK, CBattleState::COUNTER, true, *p);
	}

	// デモ・ON/OFF
	pOnOff_->validWidget(p->getDemo()?"ON_BUTTON":"OFF_BUTTON");

	// ヘルプモード
	callHelp(Unit::Help::SLG_ATK_CHECK, "SLG_ATK_CHECK", pContext);
}

void CAttack_action::setStateBackUpAtk(int nPos, CSLGContext& p)
{// 援護攻撃キャラの状態設定
	if(nPos<0) return;

	state_.setCharaData(p.getCharaData(pairBackUp_[nPos].first), CBattleState::ATTACK_BACK);
	state_.setWeaponID(pairBackUp_[nPos].second, CBattleState::ATTACK_BACK);
	calcAndSetHit(CBattleState::ATTACK_BACK, CBattleState::COUNTER, &p);
}


////////////////////////////////////////////////////////////////////////////////////
// インターフェイスへの反映メソッド
////////////////////////////////////////////////////////////////////////////////////
namespace{
__inline int getActionID(int nID, bool bCounter)
{
	switch(nID)
	{
	case Battle::AVOID:		return 2;
	case Battle::DEFENCE:	return 3;
	default:				return bCounter ? 0: 1;
	}
}
} // namespace end

void CAttack_action::initPanel(int nLeft, int nRight, bool bLeft, CSLGContext& p)
{
	// LEFT
	initPanelSide(nLeft, !bLeft, CStatusCharaVeryEasy::LEFT, p);

	// RIGHT
	initPanelSide(nRight, bLeft, CStatusCharaVeryEasy::RIGHT, p);
	
	// 援護攻撃
	initPanelBackUpAtk(bLeft,p);

	// 援護防御
	initPanelBackUpDef(!bLeft,p);

	
	// 反撃行動関係
	// 反撃時のみ動作＆表示
	// ただし、NPCが防御キャラ、行動不能の場合は表示しない
	pMain_->getWidgetRec("PANEL_BATTLESTART/HEADER_ACTION")->visible(!bNpc_ && bLeft && !state_.getCharaData(CBattleState::COUNTER)->getBattle().IsCond(Chara::CValidCond::ACTION));
	pAct_->valid(!bNpc_ && bLeft);
	pAct_->visible(!bNpc_ && bLeft);
	if(!bNpc_ && bLeft)
	{// 現状に合わせて選択
		if(state_.getWeaponID(CBattleState::COUNTER)==Battle::HIT) 
		// 反撃不能
			pAct_->validWidget("DONT_ATK");
		else // 反撃
			pAct_->validWidget("ATK");
	}
}

// メインパネル設定
void CAttack_action::initPanelSide(int nChara, bool bCounter, int nSide, CSLGContext& p)
{// 片側を更新する
	CDataCharaSLG*	pChara	= state_.getCharaData(nChara);

	// メインパネル
	GUI::CPanel* pPanel = pMain_->getWidgetCast<GUI::CPanel>(nSide==CStatusCharaVeryEasy::LEFT ?
															"PANEL_ACTION_L" : "PANEL_ACTION_R");

	// 顔
	GUI::CGraphicFace* pFace = pPanel->getWidgetCast<GUI::CGraphicFace>("CHARA");
	pFace->setFace(pChara->getBattle().getFaceID(), "DEFAULT", &p);

	// ↑以外
	updatePanelSide(nChara, nSide, pPanel, p);
}

void CAttack_action::updatePanelSide(int nChara, int nSide, GUI::CPanel* pPanel, CSLGContext& p)
{
	bool bCounter = nChara==CBattleState::COUNTER;
	// 簡易ステータス
	p.getEvent()->validStatus(true,nSide);
	p.getEvent()->getStatus(nSide).actionReset(*state_.getCharaData(nChara),
											   CStatusCharaVeryEasy::MENTAL,
											   state_.getCharaData(nChara)->getBattle().getMental());

	// 武器部分
	updatePanelSideWeapon(nChara, bCounter?CBattleState::ATTACK:CBattleState::COUNTER, pPanel, p);

	// 状態変化
	setJotai(pPanel->getWidgetCast<GUI::CPanel>("JOUTAI"),
			 state_.getCharaData(nChara)->getBattle().getValidCond());
	// 掛かっている精神
	setSpirits(pPanel->getWidgetCast<GUI::CPanel>("SPIRITS"),
			   state_.getCharaData(nChara)->getBattle().getValidSpirit(),p.getApp()->getSpirit());
}

namespace{
__inline void getWeaponText(int nID, CSLGContext& p, string& sName)
{
	switch(nID)
	{
	case Battle::AVOID:		sName="回避";		break;
	case Battle::DEFENCE:	sName="防御";		break;
	case Battle::HIT:		sName="反撃不能";	break;
	default:				sName = p.getWeaponData(nID)->getName(); break;
	}
}
} // namespace end

void CAttack_action::updatePanelSideWeapon(int nChara, int nTarget, GUI::CPanel* pPanel, CSLGContext& p)
{
	// 武器
	GUI::CText* pText = pPanel->getWidgetCast<GUI::CText>("WEAPON_NAME");
	string sName;

	// 武器名取得
	getWeaponText(state_.getWeaponID(nChara),p,sName);

	if(sName!=pText->getText())
	{// 武器名が違う時だけ良し
		pText->setText(sName);
		pText->UpdateTextAA();
	}

	// 射程マーク
	GUI::CPanelCtrl* pMark = pPanel->getWidgetCast<GUI::CPanelCtrl>("MARK_RANGE");
	setRangeMark(state_.getWeaponID(nChara), pMark, p, nChara==CBattleState::COUNTER || nChara==CBattleState::COUNTER_BACK);

	// 命中
	int nHit;
	if(state_.getWeaponID(nChara)<0){ nHit=0; }
	else
	{// 攻撃あり
		nHit = state_.getHit(nChara);
		if(state_.getWeaponID(nTarget)==Battle::AVOID) nHit/=2;
		nHit+=state_.getOffHit(nChara);
		if(nHit>200) nHit=200;
		ef(nHit<0) nHit=0;
	}
	pPanel->getWidgetCast<GUI::CNum>("HIT")->setNum(nHit);
}

void CAttack_action::setRangeMark(int nWeaponID, GUI::CPanelCtrl* pPanel, CSLGContext& p, bool bCounter)
{
	switch(nWeaponID)
	{
	case Battle::AVOID:		
	case Battle::DEFENCE:
	case Battle::HIT:
		pPanel->visible(false);
	break;

	default: 
	{
		pPanel->visible(true);
		pPanel->validWidget(p.getWeaponData(nWeaponID)->IsCore(bCounter ? nCounterDist_ : nDist_) ? "CORERANGE" : "ALLRANGE");
	}
	break;
	}
}

////////////////////////////////////////////////////////
// 援護パネル設定
////////////////////////////////////////////////////////

//////////////////////////////////////////
// 援護攻撃
//////////////////////////////////////////
void CAttack_action::initPanelBackUpAtk(bool bLeft, CSLGContext& p)
{// 援護攻撃
	GUI::CPanel* pPanel = pSupport_->getWidgetCast<GUI::CPanel>(bLeft ? "PANEL_SUPPORT_L" : "PANEL_SUPPORT_R"); 
	// とりあえず、表示消し
	pPanel->getWidget("HIT")->visible(false);
	pPanel->getWidget("WEAPON_NAME")->visible(false);
	pPanel->getWidget("MARK_RANGE")->visible(false);
	
	Task::ITaskBase* pBase=nullptr;
	if(!bLeft)
	{// 一端、武器選択ボタン停止
		pBase = pPanel->getWidget("WEAPON_SELECT");
		pBase->valid(false);
		pBase->visible(false);
	}

	// チップ表示消し
	updatePanelBackUpChipVisible(bLeft, pPanel, p);
	
	// 援護なければ、リターン
	if(state_.getCharaData(CBattleState::ATTACK_BACK)==NULL) return;

	// 援護あるでー
	if(!bLeft && pBase!=nullptr)
	{
		pBase->valid(true);
		pBase->visible(true);
	}

	// キャラチップは、敵と味方で処理が違う
	initPanelBackUpChip(pPanel,bLeft,true,p);

	if(bLeft)
	{ // 敵側だったらここで更新
		// 命中計算
		calcAndSetHit(CBattleState::ATTACK_BACK, CBattleState::COUNTER, &p);
		// GUI反映
		updatePanelBackUpAtkWeapon(pPanel,p);
	}
}

void CAttack_action::updatePanelBackUpAtkWeapon(GUI::CPanel* pPanel, CSLGContext& p)
{// 援護攻撃キャラの状態にあわせて、命中や武器を設定
	bool bEnable = state_.getCharaData(CBattleState::ATTACK_BACK)!=NULL;

	// 命中率
	GUI::CNum* pNum = pPanel->getWidgetCast<GUI::CNum>("HIT");
	pNum->visible(bEnable);
	// 武器名
	GUI::CText* pText = pPanel->getWidgetCast<GUI::CText>("WEAPON_NAME");
	pText->visible(bEnable);
	// 射程マーク
	GUI::CPanelCtrl* pMark = pPanel->getWidgetCast<GUI::CPanelCtrl>("MARK_RANGE");
	pMark->visible(bEnable);

	if(bEnable)
	{
		int nHit = (state_.getWeaponID(CBattleState::COUNTER)==Battle::AVOID
					? state_.getHit(CBattleState::ATTACK_BACK)/2 
					: state_.getHit(CBattleState::ATTACK_BACK)
					)
					+ state_.getOffHit(CBattleState::ATTACK_BACK);
		// 命中率値
		pNum->setNum(nHit > 200 ? 200 : nHit);
		// 武器名設定
		pText->setText(p.getWeaponData(state_.getWeaponID(CBattleState::ATTACK_BACK))->getName());
		pText->UpdateTextAA();
		// 射程マーク設定
		setRangeMark(state_.getWeaponID(CBattleState::ATTACK_BACK), pMark, p, false);
	}
}
//////////////////////////////////////////
// 援護防御
//////////////////////////////////////////
void CAttack_action::initPanelBackUpDef(bool bLeft, CSLGContext& p)
{// 援護防御
	GUI::CPanel* pPanel = pSupport_->getWidgetCast<GUI::CPanel>(bLeft ? "PANEL_SUPPORT_L" : "PANEL_SUPPORT_R"); 
	// とりあえず、表示消し
	pPanel->getWidget("HP")->visible(false);
	if(bLeft)
	{// 敵側だったら？？？？？？もある
		pPanel->getWidget("HP_MAX")->visible(false);
		pPanel->getWidget("HP_NOW")->visible(false);
	}
	// チップ表示消し
	updatePanelBackUpChipVisible(bLeft, pPanel, p);
	// 援護なければ、リターン
	if(state_.getCharaData(CBattleState::COUNTER_BACK)==NULL) return;

	// キャラチップは、敵と味方で処理が違う
	initPanelBackUpChip(pPanel,bLeft,false,p);

	if(bLeft) // 敵側だったらここで更新
		updatePanelBackUpDefHP(pPanel,p);
}

void CAttack_action::updatePanelBackUpDefHP(GUI::CPanel* pPanel, CSLGContext& p)
{// 援護防御キャラの状態にあわせて、HPを設定
	CDataCharaSLG* pChara = state_.getCharaData(CBattleState::COUNTER_BACK);
	// HP
	GUI::CGage* pGage = pPanel->getWidgetCast<GUI::CGage>("HP");
	pGage->visible(pChara!=NULL);
	Task::ITaskBase* pMax = pPanel->getWidget("HP_MAX");
	Task::ITaskBase* pNow = pPanel->getWidget("HP_NOW");
	
	if(pChara!=NULL)
	{// 有効ならば
		if(pMax!=NULL) pMax->visible(!pChara->getState().IsApper());
		if(pNow!=NULL) pNow->visible(!pChara->getState().IsApper());
		pGage->getCurrentNumGui()->visible(pChara->getState().IsApper());
		pGage->getMaxNumGui()->visible(pChara->getState().IsApper());
		
		const Chara::CDataCharaBattle& battle = pChara->getBattle();
		pGage->actionChangeNum(battle.getHP(), battle.getMaxHP());	
	}
	else
	{// だれもいない
		if(pMax!=NULL){ pMax->visible(false); }
		if(pNow!=NULL) pNow->visible(false);
	}
}


///////////////////////////////////////////
// チップ処理
///////////////////////////////////////////
void CAttack_action::initPanelBackUpChip(GUI::CPanel* pPanel, bool bLeft, bool bAtk, CSLGContext& p)
{// 援護キャラの状態に合わせてボタンを切り替える
	using namespace Misc;
	if(bLeft)
	{// 敵側
		pPanel = pPanel->getWidgetCast<GUI::CPanel>("CHARA");
		pPanel->visible(true);
		initPanelBackUpChipButton(pPanel,*state_.getCharaData(bAtk ? CBattleState::ATTACK_BACK : CBattleState::COUNTER_BACK),p,bAtk,true);
	}
	else
	{// 味方側
		GUI::CPanel* pChip;
		for(int i=0; i<4; i++)
		{
			if(pairBackUp_[i].first<0) break;
			pChip = pPanel->getWidgetCast<GUI::CPanel>(linkStrAndNum("CHARA",i+1));
			pChip->valid(true);
			pChip->visible(true);
			initPanelBackUpChipButton(pChip,*p.getCharaData(pairBackUp_[i].first),p,bAtk,false);
		}
		// 初期選択
		int nID = nBackUpPos_+1;
		nBackUpPos_=-1;
		// 選択済みのをそうする
		pChip = pPanel->getWidgetCast<GUI::CPanel>(linkStrAndNum("CHARA",nID));
		pChip->getWidgetCast<GUI::CButtonKeepSymbol>("CHIP")->actionRelease(&p);
	}
}

void CAttack_action::initPanelBackUpChipButton(GUI::CPanel* pPanel, CDataCharaSLG& chara, CSLGContext& p, bool bAtk, bool bEnemy)
{// キャラボタンスプライトの設定
	if(bEnemy)
	{// 敵側チップ設定
		Task::ITaskBase* pChip = pPanel->getWidget("CHIP");
		Task::ITaskBase* pChipNew = chara.getMapSymbol()->createSymbolStr("BEFORE_BOTTOM");
		// 設定位置をもらう
		pChipNew->setDrawInfo(pChip->getDrawInfo(false));
		// データ入れ替え
		pPanel->swapWidget(pChipNew,"CHIP");
	}
	else
	{// 味方側チップ設定
		GUI::CButtonSymbol* pChip = pPanel->getWidgetCast<GUI::CButtonSymbol>("CHIP");
		chara.getMapSymbol()->setButtonGui(pChip,"BUTTON_SLG");
		pChip->OnReset(&p);
	}

	// 援護回数
	GUI::CNumRemain* pRemain = pPanel->getWidgetCast<GUI::CNumRemain>("REMAIN");
	int nAttr = bAtk 
				? chara.getBattle().hasSkill(Ability::BACKUPATTACK)
				: chara.getBattle().hasSkill(Ability::BACKUPDEFENCE);
	pRemain->setMaxNum(nAttr);
	nAttr = bAtk 
			? chara.getBattle().getBackUpAttack()
			: chara.getBattle().getBackUpDefence();
	pRemain->setCurrentNum(nAttr);
}

void CAttack_action::updatePanelBackUpChipVisible(bool bLeft, GUI::CPanel* pPanel, CSLGContext& p)
{// 表示消し
	if(bLeft)
	{// 敵の表示消し
		pPanel->getWidget("CHARA")->visible(false);
	}
	else
	{// 味方の表示消し
		Task::ITaskBase* pBase = pPanel->getWidget("CHARA1");
		pBase->valid(false);
		pBase->visible(false);
		pBase = pPanel->getWidget("CHARA2");
		pBase->valid(false);
		pBase->visible(false);
		pBase = pPanel->getWidget("CHARA3");
		pBase->valid(false);
		pBase->visible(false);
		pBase = pPanel->getWidget("CHARA4");
		pBase->valid(false);
		pBase->visible(false);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end