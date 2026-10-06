#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "CGameOverUnit.h"

namespace BMW{
namespace SLG{
namespace Menu{

CGameOverUnit::~CGameOverUnit()
{
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(plane_);
}

void CGameOverUnit::Task(Task::CTaskContext* pContext)
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
			//pBlack_->Task(pContext);
			(*pContext->getDrawPlane())->BlendBltFast(plane_,0,0,c_/10);
			pPanel_->Task(pContext);
		}
	}
}

void CGameOverUnit::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_CONTINUE");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	
	// 黒プレーン
	plane_ = new CFastPlane();
	plane_->CreateSurface(640,480,false);
	plane_->Clear();
}

void CGameOverUnit::OnAction(Task::CTaskContext* pContext)
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

	default:break;
	}
}

void CGameOverUnit::setInit(const GUI::CButton::ButtonEvent& fun, Task::CTaskContext* pContext)
{
	// イベントハンドラ設定
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("SAVE"),fun,SAVE);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("MAP"),fun,MAP);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("TITLE"),fun,TITLE);
	
	setState(INTRO);
	c_.Set(0,255,10);
	pContext->getInput()->guard(true);
	//int nX,nY;
	//pContext->getInput()->getCursolPos(nX,nY);
	//CDbg().Out("%d %d",nX,nY);
	//pContext->getInput()->setCursolPos(nX,nY);
}

void CGameOverUnit::eventCursol()
{// なにもしない
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end