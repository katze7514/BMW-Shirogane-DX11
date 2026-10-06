#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"

#include "../IDRule.h"
#include "../slg_fun.h"

#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../GUI/CStatusCharaVeryEasy.h"
#include "../Action/IAction.h"

#include "CAttack_calc.h"
#include "CAttack_action.h"

namespace BMW{
namespace SLG{
namespace Attack{

CAttack_action::~CAttack_action()
{
	listTask_.clear();
	DELETE_SAFE(pCancel_);
	DELETE_SAFE(pMain_);
	DELETE_SAFE(pPlayer_);
	DELETE_SAFE(pEnemy_);
	DELETE_SAFE(pMenu_);
	DELETE_SAFE(pWeaponMenu_);
}

////////////////////////////////////////////////////////////
// タスク
////////////////////////////////////////////////////////////
namespace{
__inline void setEventButtonHolder(GUI::CPanel* pPanel, const GUI::CButton::ButtonEvent& fun)
{// キャラボタンに対するハンドラなどの設定
	GUI::CButtonHolder* pButton;
	for(int i=1; i<=4; ++i)
	{
		pButton = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARA",i))->getWidgetCast<GUI::CButtonHolder>("CHIP");
		GUI::CButton::setButtonEvent(pButton,fun,CAttack_action::CHARA1+i-1);
	}
}

} // namespace end

void CAttack_action::OnReset(Task::CTaskContext* pContext)
{// とりあえず、受け入れ口をつくる
	using namespace GUI;

	pCancel_ = new BMW::Rule::CRuleCancel(CANCEL);
	addTask(pCancel_,CANCEL_T);
	// 定義DB取得
	CGuiDefDB& db = pContext->getScene()->getGuiDefDB();
	CButton::ButtonEvent funButton;
	CPanel* pPanel;

	// メインパネル
	pMain_ = db.createInterfaceCast<CPanel>("PANEL_BATTLEMAIN");
	addTask(pMain_,MAIN);

	// 反撃メニュー
	pMenu_ = db.createInterfaceCast<GUI::CCircleMenu>("COUNTER_CIRCLE");
	pMenu_->valid(false);
	pMenu_->visible(false);
	addTask(pMenu_,MENU);
	// サークルイベントハンドラ設定
	CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CAttack_action::eventCircle);
	pMenu_->setEventHandler(funCircle);
	// ボタンイベントハンドラ設定
	funButton.set(this,&CAttack_action::eventButton);
	pMenu_->setButtonEventHandler("COUNTER",funButton,COUNTER_B);
	pMenu_->setButtonEventHandler("AVOID",funButton,AVOID_B);
	pMenu_->setButtonEventHandler("DEF",funButton,DEF_B);

	// 前バージョンで言うCTRLに対するイベントハンドラ設定
	pPanel = pMain_->getWidgetCast<CPanel>("PANEL_BATTLESTART");
	// 反撃行動選択ボタンに対するハンドラ設定
	pAct_ = pPanel->getWidgetCast<CPanelCtrl>("ACTION_BUTTON");
	CButton::setButtonEvent(pAct_->getWidgetCast<CButton>("ATK"),funButton,SELECT_B);
	CButton::setButtonEvent(pAct_->getWidgetCast<CButton>("AVOID"),funButton,SELECT_B);
	CButton::setButtonEvent(pAct_->getWidgetCast<CButton>("DEF"),funButton,SELECT_B);
	CButton::setButtonEvent(pAct_->getWidgetCast<CButton>("DONT_ATK"),funButton,SELECT_B);
	// デモON/OFF
	pOnOff_ = pPanel->getWidgetCast<CPanelCtrl>("DEMO_ONOFF");
	funButton.set(this,&CAttack_action::eventCtrl);
	// ON
	CButton::setButtonEvent(pOnOff_->getWidgetCast<CButton>("ON_BUTTON"), funButton, ON_C);
	// OFF
	CButton::setButtonEvent(pOnOff_->getWidgetCast<CButton>("OFF_BUTTON"), funButton, OFF_C);
	// GO
	CButton::setButtonEvent(pPanel->getWidgetCast<CButton>("BATTLESTART"), funButton, GO_C);

	funButton.set(this, &CAttack_action::eventBackUpAtk);
	// 味方フェイズ時援護パネル
	pPlayer_ = db.createInterfaceCast<CPanel>("PANEL_SUPPORT_PLAYER");
	pPanel = pPlayer_->getWidgetCast<CPanel>("PANEL_SUPPORT_R");
	// キャラホルダにイベントハンドラ設定
	setEventButtonHolder(pPanel,funButton);
	// こっちの援護攻撃の武器選択ボタンにハンドラ設定
	CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("WEAPON_SELECT"),funButton,SELECT);
	
	// 敵フェイズ時援護パネル
	pEnemy_ = db.createInterfaceCast<CPanel>("PANEL_SUPPORT_ENEMY");
	// キャラホルダにイベントハンドラ設定
	funButton.set(this, &CAttack_action::eventBackUpDef);
	setEventButtonHolder(pEnemy_->getWidgetCast<CPanel>("PANEL_SUPPORT_R"),funButton);

	// 武器選択
	pWeaponMenu_ = new CAttack_weapon_support();
	pContext->push(1);
	pWeaponMenu_->OnReset(pContext);
	pContext->pop();
	addTask(pWeaponMenu_,WEAPON_MENU);
	pWeaponMenu_->valid(false);
	pWeaponMenu_->visible(false);
}

void CAttack_action::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
	{
		pContext->push(-1);
		actionEnd(pContext);
	}
	break;

	case COUNTER_CANCEL:
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
		setState(NORMAL);
	break;

	default: break;
	}
}

void CAttack_action::actionEnd(Task::CTaskContext* pContext)
{
	// 簡易ステ非表示に
	const smart_ptr<Event::CEvent>& pEvent = static_cast<CSLGContext*>(pContext)->getEvent();
	pEvent->validStatus(false,CStatusCharaVeryEasy::LEFT);
	pEvent->validStatus(false,CStatusCharaVeryEasy::RIGHT);
	pContext->getInput()->guard(true);
	static_cast<CSLGContext*>(pContext)->setCirclePos(Pos::CHARA);
	getTaskListCtrl()->returnTaskList();	
}

////////////////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////////////////
void CAttack_action::eventCtrl(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{// 前回CAttack_ctrlとして定義していたやつ
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// ボタンが押された
		switch(pButton->getValue())
		{// 押されたボタンによって処理分岐
		case ON_C:
			// ONボタンが押された
			pContext->setValue(0,Flag::DEMO);
			pOnOff_->validWidget("OFF_BUTTON");
			pOnOff_->getValidWidget()->OnReset(pContext);
		break;
		
		case OFF_C:
			// OFFボタンが押された
			pContext->setValue(1,Flag::DEMO);
			pOnOff_->validWidget("ON_BUTTON");
			pOnOff_->getValidWidget()->OnReset(pContext);
		break;

		case GO_C:
			// 次はカーソルが非表示
			pContext->getInput()->cursolVisible(false);
			// 戦闘開始！
			actionGo(pContext);
			// 戦闘デモのON/OFF
			pContext->push(pContext->getValue(Flag::DEMO));
			actionEnd(pContext);			
		break;

		default: break;
		}
	}
}

void CAttack_action::eventBackUpAtk(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{// 援護攻撃キャラ選択
	if(GUI::IsRelease(pButton))
	{// ボタン押された
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 現在選択中のやつと同じか？
		if(pButton->getValue()==SELECT)
		{// 援護攻撃の武器選択ボタンが押された
			// 誰も選択されていない、選択されたやつがNPCキャラ
			// だったら、無視
			if(nBackUpPos_<0
			|| p->getCharaData(pairBackUp_[nBackUpPos_].first)->getAction()->IsNonPlayer())
				return;

			setState(SUPPORT_ATK);
			CAttack_weapon_support::WeaponEvent fun;
			fun.set(this,&CAttack_action::eventWeaponBackAtk);
			readyWeaponSelect(pairBackUp_[nBackUpPos_].first, fun, false, pContext);
		}
		else
		{// キャラ替え
			// 位置更新
			changeValidChara(pButton->getValue());
			// とりあえず、武器選択
			pairBackUp_[nBackUpPos_].second=(p->getCharaData(pairBackUp_[nBackUpPos_].first))->actionCounter(nDist_,nRealDist_,nHeight_,*p,state_.getCharaData(CBattleState::COUNTER)->getBattle().getHP(),true);
			updateBackUpAtk(*p);

			// 命中率の反映
			// 武器名の反映
			updatePanelBackUpAtkWeapon(pSupportR_,*p);
		}
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 位置更新
		nBackUpPos_=-1;
		// とりあえず、武器選択
		state_.getStatePtr(CBattleState::ATTACK_BACK)->clearData();
		// 命中率の反映
		// 武器名の反映
		updatePanelBackUpAtkWeapon(pSupportR_,*p);
	}
}

void CAttack_action::eventBackUpDef(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{// 援護防御キャラ選択
	if(GUI::IsRelease(pButton))
	{// ボタン押された
		// キャラ替え
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 位置更新
		changeValidChara(pButton->getValue());
		// キャラの設定
		state_.setCharaData(p->getCharaData(pairBackUp_[nBackUpPos_].first),CBattleState::COUNTER_BACK);
		// HPの設定
		updatePanelBackUpDefHP(pSupportR_,*p);
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 位置更新
		nBackUpPos_=-1;
		// キャラ設定
		state_.getStatePtr(CBattleState::COUNTER_BACK)->clearData();
		// HPの設定
		updatePanelBackUpDefHP(pSupportR_,*p);
	}
}

// 反撃行動選択に対するもの↓
namespace{
__inline string getCounterActionID(int nID)
{
	switch(nID)
	{
	case Battle::AVOID:		return "AVOID";
	case Battle::DEFENCE:	return "DEF";
	case Battle::HIT:		return "DONT_ATK";
	default:				return "ATK";
	}
}

} // namespace end

void CAttack_action::readyCircleCounter(Task::CTaskContext* pContext)
{
	setState(COUNTER);
	pCancel_->valid(true);
	pCancel_->setValue(COUNTER_CANCEL);
	pMain_->valid(false);
	pSupport_->valid(false);
	pMenu_->valid(true);
	pMenu_->visible(true);
	// メニューはカーソルを押したところってことで
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->setCirclePos(Pos::MOUSE);
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());

	// 現在の状態から表示ボタン選択
	switch(state_.getWeaponID(CBattleState::COUNTER))
	{
	case Battle::HIT: // 反撃不能
		pMenu_->validButton(true,"AVOID",pContext);
		pMenu_->validButton(true,"DEF",pContext);
	break;

	case Battle::AVOID: // 回避
		if(!bImCounter_) pMenu_->validButton(true,"COUNTER",pContext);
		pMenu_->validButton(true,"DEF",pContext);
	break;

	case Battle::DEFENCE: // 防御
		if(!bImCounter_) pMenu_->validButton(true,"COUNTER",pContext);
		pMenu_->validButton(true,"AVOID",pContext);
	break;

	default:
		if(!bImCounter_) pMenu_->validButton(true,"COUNTER",pContext);
		pMenu_->validButton(true,"AVOID",pContext);
		pMenu_->validButton(true,"DEF",pContext);
	break;
	}

	actionMenu(GUI::CCircleMenu::INTRO,pContext);
}

void CAttack_action::eventCircle(int nState,Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::INTRO)
	{// 登場終了
		pContext->getInput()->guard(false);
	}
	else
	{// 退場
		//removeTask(MENU);
		pMenu_->valid(false);
		pMenu_->visible(false);
		if(getState()==COUNTER_ATK)
		{// 反撃武器選択状態だったら、武器選択表示
			CAttack_weapon_support::WeaponEvent fun;
			fun.set(this,&CAttack_action::eventWeaponCounter);
			readyWeaponSelect(state_.getCharaData(CBattleState::COUNTER)->getID(), fun, true, pContext);
		}
		else
		{// 反撃武器選択ではないか、キャンセルされた
			if(getState()==COUNTER)
			{// こっちだと回避とかが選択されてるっぽいよ
				CSLGContext* p = static_cast<CSLGContext*>(pContext);
				// 命中率計算しなおし
				// メイン
				calcAndSetHit(CBattleState::ATTACK, CBattleState::COUNTER, p);
				updatePanelSideWeapon(CBattleState::ATTACK, 
									  CBattleState::COUNTER,
									  pMain_->getWidgetCast<GUI::CPanel>("PANEL_ACTION_L"),
									  *p);
				updatePanelSideWeapon(CBattleState::COUNTER, 
									  CBattleState::ATTACK,
									  pMain_->getWidgetCast<GUI::CPanel>("PANEL_ACTION_R"),
									  *p);

				// 援護攻撃があれば
				if(state_.getCharaData(CBattleState::ATTACK_BACK)!=NULL) 
				{
					calcAndSetHit(CBattleState::ATTACK_BACK, CBattleState::COUNTER, static_cast<CSLGContext*>(pContext));
					updatePanelBackUpAtkWeapon(pSupport_->getWidgetCast<GUI::CPanel>("PANEL_SUPPORT_L"),*p);
				}
				setState(NORMAL);
			}

			pContext->getInput()->guard(false);
			pMain_->valid(true);
			pSupport_->valid(true);
			pCancel_->valid(false);

			// 反撃ボタン表示変更
			pAct_->validWidget(getCounterActionID(state_.getWeaponID(CBattleState::COUNTER)));
		}
	}
}

void CAttack_action::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// ボタンが押されました
		pCancel_->valid(false);
		switch(pButton->getValue())
		{
		// 反撃が選択されたっぽい
		case COUNTER_B: setState(COUNTER_ATK); break;
		// 回避が選択されたっぽい
		case AVOID_B:	state_.setWeaponID(Battle::AVOID,CBattleState::COUNTER); setState(COUNTER); break;
		// 防御が選択されたっぽい
		case DEF_B:		state_.setWeaponID(Battle::DEFENCE,CBattleState::COUNTER); setState(COUNTER); break;
		// 反撃行動選択メニュー表示
		default:		readyCircleCounter(pContext); return; // ←ちょっとポイント？
		}
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
	}
}

void CAttack_action::readyWeaponSelect(int nID, const CAttack_weapon_support::WeaponEvent& fun, bool bCounter, Task::CTaskContext* pContext)
{
	// パネル全体を停止させる
	pMain_->valid(false);
	pSupport_->valid(false);
	pCancel_->valid(false);
	pContext->getInput()->guard(true);
	// 武器選択を呼び出すためにデータ保存
	nCtrl_ = pContext->getValue(Flag::CTRL_CHARA);
	pContext->setValue(nID,Flag::CTRL_CHARA);
	// イベントハンドラ設定
	pWeaponMenu_->setEventHandler(fun);

	// データ投下
	// 高さ
	pContext->push(bCounter ? abs(nCounterHeight_) : abs(nHeight_));
	// 実際距離
	pContext->push(nRealDist_);
	// 距離
	pContext->push(bCounter ? nCounterDist_ : nDist_);
	// 援護フラグ
	pContext->push(1);
	static_cast<CSLGContext*>(pContext)->setCirclePos(Pos::MOUSE);
	// 設定～
	pWeaponMenu_->OnInit(pContext);
	pWeaponMenu_->valid(true);
	pWeaponMenu_->visible(true);
}

void CAttack_action::eventWeaponCounter(int nState,Task::CTaskContext* pContext)
{// 武器選択メニュー
	// とりあえず、動作リストからはずす
	pWeaponMenu_->valid(false);
	pWeaponMenu_->visible(false);
	setState(NORMAL);
	// 各タスク動作開始
	pMain_->valid(true);
	pSupport_->valid(true);
	//pCancel_->valid(true);
	pContext->getInput()->guard(false);
	// データを元に戻しておく
	pContext->setValue(nCtrl_, Flag::CTRL_CHARA);

	if(nState==CAttack_weapon_support::CANCEL_EVENT)
	{// キャンセルされた
		pContext->pop();
	}
	else
	{// 決定～
		// 武器ID設定
		state_.setWeaponID(pContext->top(),CBattleState::COUNTER);
		pContext->pop();
		// 計算&表示アップデート
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 味方側
		calcAndSetHit(CBattleState::COUNTER, CBattleState::ATTACK, p);
		updatePanelSideWeapon(CBattleState::COUNTER, 
							  CBattleState::ATTACK,
							  pMain_->getWidgetCast<GUI::CPanel>("PANEL_ACTION_R"),
							  *p);

		// 敵側
		calcAndSetHit(CBattleState::ATTACK, CBattleState::COUNTER, p);
		updatePanelSideWeapon(CBattleState::ATTACK, 
							  CBattleState::COUNTER,
							  pMain_->getWidgetCast<GUI::CPanel>("PANEL_ACTION_L"),
							  *p);
		
		// 反撃ボタン表示変更
		pAct_->validWidget("ATK");
	}
}
void CAttack_action::eventWeaponBackAtk(int nState,Task::CTaskContext* pContext)
{// 援護攻撃キャラの武器選択から戻ってきた

	// とりあえず、動作リストからはずす
	pWeaponMenu_->valid(false);
	pWeaponMenu_->visible(false);
	setState(NORMAL);
	// 各タスク動作開始
	pMain_->valid(true);
	pSupport_->valid(true);
	pCancel_->valid(!bNpc_);
	pContext->getInput()->guard(false);
	// データを元に戻しておく
	pContext->setValue(nCtrl_, Flag::CTRL_CHARA);

	if(nState==CAttack_weapon_support::CANCEL_EVENT)
	{// キャンセルされた
		pContext->pop();
	}
	else
	{// 決定～
		// 武器ID設定
		pairBackUp_[nBackUpPos_].second=pContext->top();
		pContext->pop();
		// 計算～
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		updateBackUpAtk(*p);
		// 表示アップデート
		updatePanelBackUpAtkWeapon(pSupportR_,*p);
	}
}


///////////////////////////////////////////////////////
// パネル操作
///////////////////////////////////////////////////////

/////////////////////////////////////////
// 援護攻撃系一括設定
////////////////////////////////////////
void CAttack_action::updateBackUpAtk(CSLGContext& p)
{
	// キャラデータの設定
	state_.setCharaData(p.getCharaData(pairBackUp_[nBackUpPos_].first),CBattleState::ATTACK_BACK);
	// 武器データの設定
	state_.setWeaponID(pairBackUp_[nBackUpPos_].second,CBattleState::ATTACK_BACK);

	// 命中率計算
	calcAndSetHit(CBattleState::ATTACK_BACK,CBattleState::COUNTER,&p);
}

///////////////////////////////////////////////////////////
// 現在のPosから渡ってきたPosのキャラに選択状態を変更する
///////////////////////////////////////////////////////////
void CAttack_action::changeValidChara(int nPos)
{
	// 現在動作してるやつキャンセル
	if(nBackUpPos_>=0)
		pSupportR_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARA",nBackUpPos_+1))->getWidget("CHIP")->setState(GUI::CButtonKeepSymbol::NORMAL);
	// こうしーん
	nBackUpPos_=nPos;
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end