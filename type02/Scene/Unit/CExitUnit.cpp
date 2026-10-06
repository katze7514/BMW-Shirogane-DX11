#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "CExitUnit.h"

namespace BMW{
namespace Unit{

CExitUnit::~CExitUnit()
{
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(plane_);
}

void CExitUnit::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			pPanel_->Task(pContext);
			OnAction(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			(*pContext->getDrawPlane())->BlendBltFast(plane_,0,0,c_/2);
			pPanel_->Task(pContext);
		}
	}
}

void CExitUnit::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("EXIT");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this, &CExitUnit::eventButton);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("GOTITLE"),fun,TITLE);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("ENDGAME"),fun,END);

	// 一枚被せる黒
	// 黒プレーン
	plane_ = new CFastPlane();
	plane_->CreateSurface(640,480,false);
	plane_->Clear();
}

void CExitUnit::OnReset(Task::CTaskContext* pContext)
{
	// 登場設定
	setState(INTRO);
	c_.Set(0,255,10);
	pPanel_->setAlpha(c_);
	pContext->getInput()->guard(true);
	Draw::CDrawInfo info = getDrawInfo();
	pContext->getInput()->actionMove(info.getX(),info.getY(),7,Input::IInput::InputEvent(this,&CExitUnit::eventCursol));
}

void CExitUnit::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INTRO:
		pPanel_->setAlpha(++c_);
		if(c_.IsEnd())
		{
			setState(NORMAL);
			pContext->getInput()->guard(false);
		}
	break;

	default: 
		// キャンセルされたー
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
			fun_(CANCEL,pContext);
	break;
	}
}

/////////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////////
void CExitUnit::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton)) // 何か押された
		fun_(pButton->getValue(),pContext);
}

void CExitUnit::eventCursol()
{// なにもしない
}
} // namespace Unit end
} // namespace BMW end