#include "stdafx.h"

#include "../Status/status_fun.h"
#include "../Status/CStatusWeaponPanel.h"
#include "../Scene/IScene.h"
#include "../Scene/Unit/IDHelp.h"
#include "../Scene/ConstScene.h"

#include "CWeaponFactory.h"

#include "IDInter.h"
#include "CInterContext.h"
#include "CInterChara.h"

#include "ITrainBase.h"
#include "CTrainFnd.h"
#include "CTrainSkill.h"
#include "CTrainBattle.h"
#include "CTrainWeapon.h"
#include "CItemEqup.h"
#include "CItemExchange.h"

#include "CChara.h"

namespace BMW{
namespace Inter{
namespace Chara{

CChara::~CChara()
{
	pWeapon_->removeWidget("PANEL");
	DELETE_SAFE(pWeaponPanel_[0]);
	DELETE_SAFE(pWeaponPanel_[1]);
}

///////////////////////////////////////////////////
// タスク
///////////////////////////////////////////////////
namespace{
const string sPanelID[]=
{
	"STATUS_BASE",
	"STATUS_WEAPON",
	"TRAIN_FND",
	"TRAIN_SKILL",
	"TRAIN_BATTLE",
	"TRAIN_WEAPON",
	"ITEM_EQUP",
	"ITEM_EXCHANGE"
};
} // namespace end
void CChara::OnInit(Task::CTaskContext* pContext)
{
	// CANCEL
	pCancel_ = new BMW::Rule::CRuleCancel(CANCEL);
	addTask(pCancel_,CANCEL_T);

	// インターフェイス
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARA_STATUS");
	addTask(pPanel_,PANEL);

	// ステータス
	// ヘッダ
	pHeader_ = pPanel_->getWidgetCast<GUI::CPanel>("CLEARTITLE");
	// 簡易ステ
	pEasy_ = pPanel_->getWidgetCast<GUI::CPanel>("EASY_STATUS");
	// 戦闘ステ
	pBattle_ = pPanel_->getWidgetCast<GUI::CPanel>("BATTLE_STATUS");

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CChara::eventChara);
	GUI::CPanel* pMenu = pPanel_->getWidgetCast<GUI::CPanel>("MENU");
	// キャラ替え
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("CHARALEFT"),funButton,C_BACK);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("CHARARIGHT"),funButton,C_FORWARD);
	// メニュー
	// イベントハンドラ設定
	funButton.set(this,&CChara::eventMenu);
	// ステータス
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("STATUS_BASE"), funButton, M_BASE);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("STATUS_WEAPON"), funButton, M_WEAPON);
	// 養成
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("YOUSEI_FND"), funButton, T_FND);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("YOUSEI_SKILL"), funButton, T_SKILL);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("YOUSEI_BATTLE"), funButton, T_BATTLE);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("YOUSEI_WEAPON"), funButton, T_WEAPON);
	// アイテム
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("ITEM"), funButton, I_EQUP);
	// 戻る
	//GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("BACK"), funButton, M_BACK);

	// 各種パネル
	pCtrl_ = new GUI::CPanelCtrl();
	pPanel_->swapWidget(pCtrl_,"CHARAPANEL");
	// 各パネルを生成
	// 基本ステータス
	pBase_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_BASICSTATUS");
	pCtrl_->addWidget(pBase_, sPanelID[M_BASE]);
	// 武器ステータス
	pWeapon_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_WEAPONSTATUS");
	pCtrl_->addWidget(pWeapon_, sPanelID[M_WEAPON]);
	pWeaponPanel_[0] = new Status::CStatusWeaponPanel();
	pWeaponPanel_[0]->OnInit(pContext);
	pWeaponPanel_[1] = new Status::CStatusWeaponPanel();
	pWeaponPanel_[1]->OnInit(pContext);
	pWeapon_->swapWidget(pWeaponPanel_[0],"PANEL");
	funButton.set(this,&CChara::eventWeapon);
	GUI::CButton::setButtonEvent(pWeapon_->getWidgetCast<GUI::CPanel>("CHANGE")->getWidgetCast<GUI::CButton>("BUTTON"), funButton, 0);
	
	ITrainBase::TrainEvent funTrain;
	funTrain.set(this,&CChara::eventStatus);
	// 基礎養成
	CTrainFnd* pTrainFnd = new CTrainFnd();
	pCtrl_->addWidget(pTrainFnd, sPanelID[T_FND]);
	pTrainFnd->setEventHandler(funTrain);
	pTrainFnd->OnInit(pContext);
	// 技能養成
	CTrainSkill* pTrainSkill = new CTrainSkill();
	pCtrl_->addWidget(pTrainSkill, sPanelID[T_SKILL]);
	pTrainSkill->setEventHandler(funTrain);
	pTrainSkill->OnInit(pContext);
	// 戦闘養成
	CTrainBattle* pTrainBattle = new CTrainBattle();
	pCtrl_->addWidget(pTrainBattle, sPanelID[T_BATTLE]);
	pTrainBattle->setEventHandler(funTrain);
	pTrainBattle->OnInit(pContext);
	// 武器養成
	CTrainWeapon* pTrainWeapon = new CTrainWeapon();
	pCtrl_->addWidget(pTrainWeapon, sPanelID[T_WEAPON]);
	pTrainWeapon->setEventHandler(funTrain);
	pTrainWeapon->OnInit(pContext);
	// アイテム装備
	CItemEqup* pItemEqup = new CItemEqup();
	pCtrl_->addWidget(pItemEqup, sPanelID[I_EQUP]);
	pItemEqup->setEventHandler(funTrain);
	pItemEqup->OnInit(pContext);
	// アイテム交換
	CItemExchange* pItemExchange = new CItemExchange();
	pCtrl_->addWidget(pItemExchange, sPanelID[I_EXCHANGE]);
	nItemExchange_ = pCtrl_->getID(sPanelID[I_EXCHANGE]);
	pItemExchange->setEventHandler(funTrain);
	pItemExchange->OnInit(pContext);
}

void CChara::OnReset(Task::CTaskContext* pContext)
{
	if(pContext->getValue(Flag::TARGET_CHARA)>=0)
	{
		// ポップアップクリア
		pContext->getApp()->getFoward()->clearPopUp();
		pContext->setValue(-1,Flag::EXCHANGE_CHARA);
		CInterChara* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData();
		// ステータス更新
		Status::setClearHeaderInter(pHeader_,*pContext);
		Status::setEasyStatus(pEasy_, *pChara, *pContext);
		Status::setBattleStatus(pBattle_, *pChara->getData());
		Status::setStatusBasic(pBase_,*pChara,*pContext);
		updateWeapon(pChara->getData(),*pContext);
		// パネル更新
		pCtrl_->OnReset(pContext);
		// とりあえず、基本ステータス
		pCtrl_->validWidget(sPanelID[M_BASE]);
		setState(NORMAL);
	}
	else{
		getTaskListCtrl()->returnTaskList();
	}
}

void CChara::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
		pContext->getInput()->guard(true);
		getTaskListCtrl()->returnTaskList();
		setState(NORMAL);
	break;

	default: 
	//{
	//	Draw::CDrawInfo info = pCtrl_->getValidWidget()->getParent()->getDrawInfo();
	//	CDbg().Out("%d %d",info.getX(),info.getY());
	//}
	break;
	}
}
///////////////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////////////
// キャラ替え
void CChara::eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押されたで
		// アイテム交換時に押されたら無視
		if(pCtrl_->getWidgetCtrl().getState()==nItemExchange_) return;

		CInterContext* p = static_cast<CInterContext*>(pContext);
		if(pButton->getValue()==C_BACK) p->backLoopChara();
		else							p->nextLoopChara();

		p->setTargetChara(*p->getCurrentChara());
		// 現在選択中のパネルを維持
		int nCurrent = pCtrl_->getValidWidget()->getTaskPriority();
		OnReset(p);
		pCtrl_->validWidget(nCurrent);
	}
}
// パネル選択ボタン用
void CChara::eventMenu(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// おされたで
		// アイテム交換時に押されたら無視
		if(pCtrl_->getWidgetCtrl().getState()==nItemExchange_) return;

		pCtrl_->validWidget(sPanelID[pButton->getValue()]);

		// ヘルプモード
		switch(pButton->getValue())
		{
		// 基礎養成
		case T_FND:	Unit::Help::callHelp(Unit::Help::INTER_FUND,"INTER_FUND",pContext); break;
		// 技能養成
		case T_SKILL: Unit::Help::callHelp(Unit::Help::INTER_SKILL,"INTER_SKILL",pContext); break;
		// 戦闘養成
		case T_BATTLE:	Unit::Help::callHelp(Unit::Help::INTER_BATTLE,"INTER_BATTLE",pContext);	break;
		// 武器養成
		case T_WEAPON: Unit::Help::callHelp(Unit::Help::INTER_WEAPON,"INTER_WEAPON",pContext); break;
		// アイテム装備
		case I_EQUP: Unit::Help::callHelp(Unit::Help::INTER_ITEM, "INTER_ITEM",pContext); break;

		default: break;
		}
	}
}

void CChara::eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 武器パネル交代ボタンが押された
		// パネルの入れ替え
		nWeapon_=1-nWeapon_;
		pWeapon_->getWidgetCast<GUI::CPanel>("CHANGE")->getWidgetCast<GUI::CNum>("PAGE")->setNum(nWeapon_+1);
		pWeapon_->swapWidget(pWeaponPanel_[nWeapon_],"PANEL",false);
	}
}

// 養成反応用
void CChara::eventStatus(int nStatus, Task::CTaskContext* pContext)
{
	switch(nStatus)
	{
	case S_HEAD: 
		Status::setClearHeaderInter(pHeader_,*pContext);
	break;

	case S_EASY:
		Status::setEasyStatus(pEasy_, *static_cast<CInterContext*>(pContext)->getTargetCharaData(), *pContext);
		pCtrl_->getWidget(sPanelID[T_BATTLE])->OnReset(pContext);
	break;

	case S_BATTLE:
	{
		BMW::Chara::CDataCharaInter* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData();
		Status::setBattleStatus(pBattle_, *pChara);
		pCtrl_->getWidget(sPanelID[T_BATTLE])->OnReset(pContext);
		static_cast<CItemEqup*>(pCtrl_->getWidget(sPanelID[I_EQUP]))->updateStatus(pChara);
	}
	break;

	case S_BASE:
		Status::setStatusBasic(pBase_,*static_cast<CInterContext*>(pContext)->getTargetCharaData(), *pContext);
	break;

	case S_WEAPON:
		updateWeapon(static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData(),*pContext);
		pCtrl_->getWidget(sPanelID[T_WEAPON])->OnReset(pContext);
	break;

	case S_ITEM_START:
		// アイテム交換準備
		// キャンセル動作が変わったり変わらなかったり
		pCancel_->valid(false);
		pCtrl_->validWidget(sPanelID[I_EXCHANGE]);
		pCtrl_->getValidWidget()->OnReset(pContext);
	break;

	case S_ITEM_END:
		// アイテム交換終了ー
		// キャンセル動作が変わったり変わらなかったり
		pCancel_->valid(true);
		pCtrl_->validWidget(sPanelID[I_EQUP]);		
		pCtrl_->getValidWidget()->OnReset(pContext);
	break;

	case S_BP:
		static_cast<CTrainBattle*>(pCtrl_->getWidget(sPanelID[T_BATTLE]))->actionUpdateBP(pContext);
		static_cast<CTrainWeapon*>(pCtrl_->getWidget(sPanelID[T_WEAPON]))->actionUpdateBP(pContext);
	break;

	case S_FP:
		static_cast<CTrainFnd*>(pCtrl_->getWidget(sPanelID[T_FND]))->actionUpdateFP(pContext);
		static_cast<CTrainSkill*>(pCtrl_->getWidget(sPanelID[T_SKILL]))->actionUpdateFP(pContext);
	break;

	default: break;
	}
}

/////////////////////////////////////////////////
// 設定
/////////////////////////////////////////////////
void CChara::updateWeapon(BMW::Chara::CDataCharaInter* pChara, Task::CTaskContext& p)
{
	GUI::CPanel* pPanel		= pWeaponPanel_[0]->getPanel();
	GUI::CPanelCtrl* pCtrl	= pWeaponPanel_[0]->getDetail();
	nWeapon_=0;

	pPanel->validAll(false);
	pPanel->visibleAll(false);
	pCtrl->visible(true);

	// とりあえず、チェンジは使わない
	GUI::CPanel* pChange = pWeapon_->getWidgetCast<GUI::CPanel>("CHANGE");
	pChange->valid(false);
	pChange->visible(false);

	GUI::CPanel* pLine;
	Weapon::CDataWeaponBattle* pWeapon;
	CWeaponFactory fct;
	// ランク付けのために一度生成してしまう
	pChara->beginWeapon();
	while(!pChara->endWeapon()) fct.createWeapon(*pChara->nextWeapon(),pChara,p);
	// ランク反映
	fct.updateRank();

	// GUIへの反映
	int nCount=1;
	pChara->beginWeapon();
	while(!pChara->endWeapon())
	{
		// 武器データ取得
		pWeapon = fct.createWeapon(*pChara->nextWeapon(),pChara,p);
		if(pWeapon==NULL) continue;

		if(nCount>=4)
		{// 次のパネル
			pCtrl->validWidget("DETAIL1");
			nCount=1;
			pPanel = pWeaponPanel_[1]->getPanel();
			pPanel->validAll(false);
			pPanel->visibleAll(false);
			pCtrl = pWeaponPanel_[1]->getDetail();
			pCtrl->visible(true);
			// 切り替えボタンの設定
			pChange->valid(true);
			pChange->visible(true);
			pChange->getWidgetCast<GUI::CNum>("PAGE")->setNum(1);
			nWeapon_=0;
		}
		ef(nCount>=7) break; // 7つ以上は対応してない

		pLine=pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("LINE",nCount));
		// 武器行
		BMW::Status::setStatusWeaponLine(pLine,	*pWeapon);
		pLine->valid(nCount<=pChara->sizeWeapon());
		pLine->visible(nCount<=pChara->sizeWeapon());
		// 詳細
		BMW::Status::setStatusWeaponDetail(pCtrl->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("DETAIL",nCount++)),
											*pWeapon,pChara->getEN(),pChara->getMental());

	}
	// とりあえず、一枚目
	pWeapon_->swapWidget(pWeaponPanel_[0],"PANEL",false);
	// とりあえず、一番目を表示
	pCtrl->validWidget("DETAIL1");
}


} // namespace Chara end
} // namespace Inter end
} // namespace BMW end