#include "stdafx.h"

#include "../../mode.h"
#include "../../Scene/IScene.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDSLG.h"
#include "../CSLGScene.h"
#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "CSave_rule.h"

namespace BMW{
namespace SLG{
namespace Save{

CSave_rule::~CSave_rule()
{
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(plane_);
}

void CSave_rule::OnReset(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("SAVELOAD");

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSave_rule::eventButton);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("SAVE"),fun,SAVE);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("LOAD"),fun,LOAD);

	// 黒プレーン
	plane_ = new CFastPlane();
	plane_->CreateSurface(640,480,false);
	plane_->Clear();
}

void CSave_rule::OnInit(Task::CTaskContext* pContext)
{		
	// 登場設定
	setState(INTRO);

	c_.Set(0,255,10);
	pPanel_->valid(false);
	pPanel_->setAlpha(c_);
	int nX,nY;
	pContext->getInput()->getCursolPos(nX,nY);
	pPanel_->setX(nX-45);
	pPanel_->setY(nY);
}

void CSave_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INTRO:
		pPanel_->setAlpha(++c_);
		if(c_.IsEnd())
		{
			pContext->getInput()->guard(false);
			pPanel_->valid(true);
			setState(NORMAL);

			// ヘルプモード
			Unit::Help::callHelp(Unit::Help::SLG_SAVE,"SLG_SAVE",pContext);
		}
	break;

	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// キャンセルされた
			eventCancel(pContext);
		}
	break;

	default:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CSave_rule::callTaskAction(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}
void CSave_rule::callTaskDraw(Task::CTaskContext* pContext)
{
	(*pContext->getDrawPlane())->BlendBltFast(plane_,0,0,c_/2);
	pPanel_->Task(pContext);
}

/////////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////////
void CSave_rule::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 何か押された
		switch(pButton->getValue())
		{
		case SAVE:
			setState(END);
			smart_ptr_static_cast<CSLGScene>(pContext->getScene())->continueSave();
			pContext->push(1);
			pContext->getInput()->guard(true);
			pPanel_->valid(false);
		break;

		default:
		{// LOAD読み直しっす
			string s;
			s = sSaveFolder + "\\";
			s+=sContinue;
			if(CDir().IsFileExist(s))
			{// セーブデータが存在していれば
				pContext->push(Victory::LOSE);
				pContext->setValue(-1,Flag::ID);
				pContext->getScene()->setState(CSLGScene::END);
				pContext->getInput()->cursolVisible(false);
				pContext->getInput()->guard(true);
				pPanel_->valid(false);
			}
		}	
		break;
		}
	}
}

void CSave_rule::eventCancel(Task::CTaskContext* pContext)
{// キャンセルされた
	pPanel_->valid(false);
	pContext->push(0);
	pContext->getInput()->guard(true);
	setState(END);
}

} // namespace Save end
} // namespace SLG end
} // namespace BMW end