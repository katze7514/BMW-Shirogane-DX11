#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/CYesNoUnit.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDRule.h"
#include "../IDSLG.h"
#include "../CSLGScene.h"

#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "../Sally/CSally_view.h"

#include "CMenu_turn.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_turn::OnReset(Task::CTaskContext* pContext)
{// 初期化
	// キャンセル
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CANCEL);
	addTask(pCancel,CANCEL_TASK);

	// メニュー生成
	pMenu_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CCircleMenu>("TURN_CIRCLE");
	addTask(pMenu_,MENU);

	// イベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CMenu_turn::eventCircle);
	pMenu_->setEventHandler(funCircle);

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this, &CMenu_turn::eventButton);
	pMenu_->setButtonEventHandler("PHASEEND",fun,PHASE);
	pMenu_->setButtonEventHandler("VICTORY",fun,VICTORY);
	pMenu_->setButtonEventHandler("SEARCH",fun,SEARCH);
	pMenu_->setButtonEventHandler("SYSTEM",fun,SYSTEM);
	pMenu_->setButtonEventHandler("SAVE",fun,SAVE);
	pMenu_->setButtonEventHandler("EXIT",fun,EXIT);
}

void CMenu_turn::OnInit(Task::CTaskContext* pContext)
{// 数値の設定
	getTask(CANCEL_TASK)->valid(true);
	setState(NORMAL);
	// マップクリックカーソル消し
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	if(p->getTargetMapChip()!=NULL)
		p->getTargetMapChip()->getMapChipState()->setState(GUI::CButton::NORMAL);

	// ターンメニューはカーソル位置に出現
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());

	pMenu_->validButton(true,"PHASEEND",pContext);
	pMenu_->validButton(true,"VICTORY",pContext);
	pMenu_->validButton(true,"SEARCH",pContext);
	pMenu_->validButton(true,"SYSTEM",pContext);
	pMenu_->validButton(true,"SAVE",pContext);
	pMenu_->validButton(true,"EXIT",pContext);
	actionMenu(GUI::CCircleMenu::INTRO,pContext);
}

void CMenu_turn::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL: actionCall(-1,pContext); break;

	case CANCEL_S: 
		static_cast<CSLGContext*>(pContext)->getEvent()->validYesNo(false);
		OnInit(pContext);
	break;

	case CALL_S:
		pContext->getInput()->guard(true);
		static_cast<CSLGContext*>(pContext)->getEvent()->validYesNo(false);
		// フェイズ切り替えフラグを立てる
		pContext->setValue(1,Flag::PHASE_CHANGE);
		getTaskListCtrl()->callTaskList(Rule::PHASE_END,true);
		setState(NORMAL);
	break;

	default: break;
	}
}

void CMenu_turn::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::PHASE_END: // フェーズエンド
		getTaskListCtrl()->returnTaskList();
	break;

	case Rule::SAVE_RULE: // セーブルール
		// セーブされたかどうかで遷移
		if(pContext->top()) // セーブされたらターンメニュー終了
			getTaskListCtrl()->returnTaskList();
		else
			OnInit(pContext);

		pContext->pop();
	break;
	
	case Rule::SALLY_VIEW: // 部隊表
		if(pContext->top()>=0)
		{// 誰かが決定されたら、そのままリターン
			pContext->pop();
			getTaskListCtrl()->returnTaskList();
		}
		else
		{// そうじゃなければ、もう一度ターンメニュー
			pContext->pop();
			OnInit(pContext);
		}
	break;

	// それ以外
	//case Rule::VICTORY_VIEW:	OnInit(pContext);	break;
	//case Rule::SYSTEM_RULE:		OnInit(pContext);	break;
	//case Rule::EXIT_RULE:		OnInit(pContext);	break;	
	default: OnInit(pContext);	break;
	}
}

////////////////////////////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////////////////////////////
void CMenu_turn::eventCircle(int nState,Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::INTRO)
	{// 登場終了
		pContext->getInput()->guard(false);
		// ヘルプモード
		Unit::Help::callHelp(Unit::Help::SLG_TURN_MENU,"SLG_TURN_MENU",pContext);
	}
	else
	{// 退場終了
		if(getState()==CALL || getState()==CALL_S)
		{// 現在の状態に合わせて処理分岐
			if(nCall_>=0)	getTaskListCtrl()->callTaskList(nCall_,true);
			else			getTaskListCtrl()->returnTaskList();
		}
		ef(getState()==PHASE_S)
		{
			pContext->getInput()->guard(false);
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			p->getEvent()->validYesNo(true);
			setState(NORMAL);
		}
	}
}

void CMenu_turn::eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// なんか押されたで
		switch(pButton->getValue())
		{
		case PHASE:
		{
			// フェーズ切り替え確認
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			int n = p->getNoActionChara();

			if(n>0)
			{// いれば確認
				getTask(CANCEL_TASK)->valid(false);
				setState(PHASE_S);
				Unit::CYesNoUnit& unit = p->getEvent()->getYesNoUnit();
				// イベントハンドラ設定
				Unit::CYesNoUnit::CancelEvent can;
				can.set(this,&CMenu_turn::eventPhaseCancel);
				unit.setCancelHandler(can);
				GUI::CButton::ButtonEvent fun;
				fun.set(this,&CMenu_turn::eventPhaseOK);
				unit.setButtonHandler(fun,YES,NO);
				unit.setX(p->getCircleX());
				unit.setY(p->getCircleY());
				// 出現
				unit.setIntro(Unit::CYesNoUnit::PHASE,p,n);
				actionMenu(GUI::CCircleMenu::EXIT,pContext);
				
			}
			else
			{// いなければそのままフェイズ切り替え
				// フェイズ切り替えフラグを立てる
				pContext->setValue(1,Flag::PHASE_CHANGE);
				actionCall(Rule::PHASE_END,pContext);
			}
		}
		break;

		case VICTORY:	actionCall(Rule::VICTORY_VIEW,pContext);	break;
		case SEARCH:
			pContext->push(Sally::CSally_view::NORMAL);
			actionCall(Rule::SALLY_VIEW,pContext);
		break;
		case SYSTEM:	actionCall(Rule::SYSTEM_RULE,pContext);		break;
		case SAVE:		actionCall(Rule::SAVE_RULE,pContext);		break;
		case EXIT:		actionCall(Rule::EXIT_RULE,pContext);		break;
		default: break;
		}
	}
}

void CMenu_turn::eventPhaseOK(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(pButton->getValue()==YES)
			setState(CALL_S);
		else
			setState(CANCEL_S);
	}
}

void CMenu_turn::eventPhaseCancel(Task::CTaskContext* pContext)
{
	// キャンセルされた！
	setState(CANCEL_S);
}


////////////////////////////////////////////////////////////////////////
// 操作
////////////////////////////////////////////////////////////////////////
void CMenu_turn::actionCall(int nState,Task::CTaskContext* pContext)
{
	// 呼び出すものがある時はコール
	nCall_=nState;
	actionMenu(GUI::CCircleMenu::EXIT,pContext);
	setState(CALL);
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end