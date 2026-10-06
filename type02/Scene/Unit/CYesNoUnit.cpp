#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "CYesNoUnit.h"

namespace BMW{
namespace Unit{

CYesNoUnit::~CYesNoUnit()
{
	DELETE_SAFE(pPanel_);
}

void CYesNoUnit::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction()&&IsValid()) OnAction(pContext);
	pPanel_->Task(pContext);
}

void CYesNoUnit::OnInit(Task::CTaskContext* pContext)
{
	// ダイアログ生成
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("YESNO");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

void CYesNoUnit::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// 大したあれでもないので、直接キャンセルは叩いてしまう
			fun_(pContext);
			setState(END);
		}
	break;

	case INTRO: // 登場時
		setAlpha(++c_);
		if(c_.IsEnd())
		{
			setState(NORMAL);
			pContext->getInput()->guard(false);
		}
	break;

	default: break;
	}
}

void CYesNoUnit::setIntro(int nAsk, Task::CTaskContext* pContext, int nNum)
{
	setAsk(nAsk);

	GUI::CText* pText = pPanel_->getWidgetCast<GUI::CText>("CHARA_NUMBER");
	pText->visible(nNum>0);
	if(nNum>0)
	{// nNumに設定されていれば
		CStringScanner::NumToString(nNum,pText->getText());
		pText->UpdateTextAA();
		// 数字位置
		if(nAsk==PHASE) pText->setX(-61);
		ef(nAsk==SALLY2) pText->setX(-29);
	}
	setState(INTRO);
	pContext->getInput()->guard(true);
	c_.Set(0,255,10);
	setAlpha(c_);

	Input::IInput::InputEvent funInput;
	funInput.set(this,&CYesNoUnit::eventCursol);
	Draw::CDrawInfo info = getDrawInfo();
	pContext->getInput()->actionMove(info.getX(),info.getY(),7,funInput);
}

void CYesNoUnit::setButtonHandler(const GUI::CButton::ButtonEvent& fun, int nYes, int nNo)
{
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("YES"),fun,nYes);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("NO"),fun,nNo);
}

void CYesNoUnit::setAsk(int nAsk)
{
	pPanel_->getWidget("CAUTION")->visible(nAsk>=0);
	GUI::CPanelCtrl* pAsk = pPanel_->getWidgetCast<GUI::CPanelCtrl>("ASK");
	if(nAsk>=0)
	{
		pAsk->validWidget(nAsk);
		pAsk->visible(true);
	}
	else
	{ pAsk->visible(false); }
}

} // namespace Unit end
} // namespace BMW end