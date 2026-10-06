#include "stdafx.h"

#include "Event/CEventButton.h"

#include "CButton.h"

namespace BMW{
namespace GUI{

CButton::CButton()
{
	pEvent_.Add(new CEventButton());
	pEvent_->setEventTask(smart_ptr<Task::ITaskBase>(this,false));
}

void CButton::actionOverIn(CTaskContext* pContext)
{
	IButton::actionOverIn(pContext);
	pEvent_->setState(OVER_IN);
	eventFun(pEvent_,pContext);
}

void CButton::actionOverOut(CTaskContext* pContext)
{
	IButton::actionOverOut(pContext);
	pEvent_->setState(OVER_OUT);
	eventFun(pEvent_,pContext);
}

void CButton::actionPress(CTaskContext* pContext)
{
	IButton::actionPress(pContext);
	pEvent_->setState(PRESS_E);
	eventFun(pEvent_,pContext);
}

void CButton::actionRelease(CTaskContext* pContext)
{
	IButton::actionRelease(pContext);
	pEvent_->setState(RELEASE);
	eventFun(pEvent_,pContext);
	pContext->getApp()->getSeDB().Play("OK");
}


/////////////////////////////////////////////////////
// 設定子
/////////////////////////////////////////////////////
void CButton::setButtonEvent(GUI::CButton* pButton, const ButtonEvent& fun, int nValue)
{
	// イベントハンドラ
	pButton->setEventHandler(fun);
	// 値
	pButton->getEvent()->setValue(nValue);
}

} // namespace GUI end
} // namespace BMW end