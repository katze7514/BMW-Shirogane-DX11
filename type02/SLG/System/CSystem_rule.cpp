#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "CSystem_rule.h"

namespace BMW{
namespace SLG{
namespace System{

void CSystem_rule::OnReset(Task::CTaskContext* pContext)
{
	// CANCEL_T
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	addTask(pCancel,CANCEL_T);
	pCancel->setValue(CANCEL);

	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// インターフェイスの取得
	GUI::CPanel* pPanel = db.createInterfaceCast<GUI::CPanel>("SYSTEM");
	addTask(pPanel,PANEL);

	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSystem_rule::eventOnOff);
	
	// グリッドボタン設定
	pGrid_ = pPanel->getWidgetCast<GUI::CPanel>("ON_GRID");
	// イベントハンドラ設定
	GUI::CButton::setButtonEvent(pGrid_->getWidgetCast<GUI::CButton>("ON"),fun,GRID_ON);
	GUI::CButton::setButtonEvent(pGrid_->getWidgetCast<GUI::CButton>("OFF"),fun,GRID_OFF);

	// フレームボタン設定
	pFrame_ = pPanel->getWidgetCast<GUI::CPanel>("ON_FRAME");
	// イベントハンドラ設定
	GUI::CButton::setButtonEvent(pFrame_->getWidgetCast<GUI::CButton>("ON"),fun,FRAME_ON);
	GUI::CButton::setButtonEvent(pFrame_->getWidgetCast<GUI::CButton>("OFF"),fun,FRAME_OFF);

	// デモOFF時音楽切り替えボタン設定
	pDemo_ = pPanel->getWidgetCast<GUI::CPanel>("ON_MUSICCHANGE");
	// イベントハンドラ設定
	GUI::CButton::setButtonEvent(pDemo_->getWidgetCast<GUI::CButton>("ON"),fun,DEMO_ON);
	GUI::CButton::setButtonEvent(pDemo_->getWidgetCast<GUI::CButton>("OFF"),fun,DEMO_OFF);

	// ヘルプボタン設定
	pHelp_ = pPanel->getWidgetCast<GUI::CPanel>("ON_HELP");
	// イベントハンドラ設定
	GUI::CButton::setButtonEvent(pHelp_->getWidgetCast<GUI::CButton>("ON"),fun,HELP_ON);
	GUI::CButton::setButtonEvent(pHelp_->getWidgetCast<GUI::CButton>("OFF"),fun,HELP_OFF);

	fun.set(this,&CSystem_rule::eventUpDown);
	// BGM
	pBgm_ = pPanel->getWidgetCast<GUI::CPanel>("VOLUME_BGM");
	// イベントハンドラ設定
	for(int i=0; i<=10; ++i)
		GUI::CButton::setButtonEvent(pBgm_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_", i)),fun,BGM_0+i);

	// SE
	pSe_ = pPanel->getWidgetCast<GUI::CPanel>("VOLUME_SE");
	// イベントハンドラ設定
	for(int i=0; i<=10; ++i)
		GUI::CButton::setButtonEvent(pSe_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_", i)),fun,SE_0+i);

	// OK
	fun.set(this,&CSystem_rule::eventOK);
	GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("OK"),fun,0);
}

namespace{
__inline void exchangeOnOff(GUI::CPanel* pPanel, bool bOn, Task::CTaskContext* pContext)
{// ON→OFF、もしくは、OFF→ONの切り替え
	Task::ITaskBase* pBase;
	pBase = pPanel->getWidget(bOn?"ON":"OFF");
	pBase->setState(GUI::CButton::PRESS);
	pBase->valid(false);
	pBase = pPanel->getWidget(bOn?"OFF":"ON");
	pBase->OnReset(pContext);
	pBase->valid(true);
}

} // namespace end

void CSystem_rule::OnInit(Task::CTaskContext* pContext)
{// 現在のパラメタから値を設定

	setState(NORMAL);
	pContext->getInput()->guard(false);

	BMW::Save::CGlobalData& data = pContext->getApp()->getGlobal();
	// グリッド
	exchangeOnOff(pGrid_,data.IsGrid(),pContext);
	// フレーム
	exchangeOnOff(pFrame_,data.IsSkip(),pContext);
	// デモオフ時音切り替え
	exchangeOnOff(pDemo_,data.IsDemoOff(),pContext);
	// デモオフ時音切り替え
	exchangeOnOff(pHelp_,data.IsHelp(),pContext);

	// バーの位置
	// BGM
	nBgm_ = data.getBGM()/10;
	setBgmVolume();
	// SE
	nSe_ = data.getSE()/10;
	setSeVolume();
}

void CSystem_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
		// この時は単にリターン
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(true);
	break;

	default: break;
	}
}

void CSystem_rule::actionOK(Task::CTaskContext* pContext)
{// 現在設定された状態を、データに保持する

	BMW::Save::CGlobalData& data = pContext->getApp()->getGlobal();

	data.grid(!pGrid_->getWidget("ON")->IsValid());
	data.skip(!pFrame_->getWidget("ON")->IsValid());
	data.demoOff(!pDemo_->getWidget("ON")->IsValid());
	data.help(!pHelp_->getWidget("ON")->IsValid());

	data.setBGM(nBgm_*10);
	data.setSE(nSe_*10);

	// かつ、実際に値を反映
	pContext->getApp()->getGlobal().setData(pContext);
}

// イベントハンドラ
void CSystem_rule::eventOnOff(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// なんかおされたで
		switch(pButton->getValue())
		{
			case GRID_ON:	exchangeOnOff(pGrid_, true, pContext);		break;
			case GRID_OFF:	exchangeOnOff(pGrid_, false, pContext);		break;
			case FRAME_ON:	exchangeOnOff(pFrame_, true, pContext);		break;
			case FRAME_OFF: exchangeOnOff(pFrame_, false, pContext);	break;
			case DEMO_ON:	exchangeOnOff(pDemo_, true, pContext);		break;
			case DEMO_OFF:	exchangeOnOff(pDemo_, false, pContext);		break;
			case HELP_ON:	exchangeOnOff(pHelp_, true, pContext);		break;
			case HELP_OFF:	exchangeOnOff(pHelp_, false, pContext);		break;
			default: break;
		}
	}
}

void CSystem_rule::eventUpDown(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// なんかおされたで
		switch(pButton->getValue())
		{
		case BGM_0:
		case BGM_1:
		case BGM_2:
		case BGM_3:
		case BGM_4:
		case BGM_5:
		case BGM_6:
		case BGM_7:
		case BGM_8:
		case BGM_9:
		case BGM_10:
			nBgm_ = pButton->getValue()-BGM_0;
			setBgmVolume();
		break;

		case SE_0:
		case SE_1:
		case SE_2:
		case SE_3:
		case SE_4:
		case SE_5:
		case SE_6:
		case SE_7:
		case SE_8:
		case SE_9:
		case SE_10:
			nSe_ = pButton->getValue()-SE_0;
			setSeVolume();
		break;

		default: break;
		}
	}
}

void CSystem_rule::setBgmVolume()
{
	if(pCurBgm_!=NULL)
	{// 動作を元に戻す
		pCurBgm_->valid(true);
		pCurBgm_->setState(GUI::IButton::NORMAL);
	}
	// 現在の音量表示を変更
	pCurBgm_ = pBgm_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_", nBgm_));
	pCurBgm_->valid(false);
	pCurBgm_->setState(GUI::IButton::PRESS);
}

void CSystem_rule::setSeVolume()
{
	if(pCurSe_!=NULL)
	{// 動作を元に戻す
		pCurSe_->valid(true);
		pCurSe_->setState(GUI::IButton::NORMAL);
	}
	// 現在の音量表示を変更
	pCurSe_ = pSe_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_", nSe_));
	pCurSe_->valid(false);
	pCurSe_->setState(GUI::IButton::PRESS);
}

void CSystem_rule::eventOK(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 決定～
		actionOK(pContext);
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(true);
	}
}


} // namespace System end
} // namespace SLG end
} // namespace BMW end