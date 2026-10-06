#include "stdafx.h"

#include "../Scene/IScene.h"
#include "CStatusWeaponPanel.h"

namespace BMW{
namespace Status{

void CStatusWeaponPanel::Task(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}

void CStatusWeaponPanel::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("WEAPON_PANEL");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pDetail_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("DETAIL");
	pDetail_->setParent(smart_ptr<Task::ITaskBase>(this,false));

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CStatusWeaponPanel::eventWeapon);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetRecCast<GUI::CButton>("LINE1/CURSOL"),fun,0);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetRecCast<GUI::CButton>("LINE2/CURSOL"),fun,1);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetRecCast<GUI::CButton>("LINE3/CURSOL"),fun,2);
}

///////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////
void CStatusWeaponPanel::eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsOverIn(pButton))
	{// マウスオーバー
		pDetail_->validWidget(pButton->getValue());
	}
	ef(GUI::IsOverOut(pButton))
	{
		pDetail_->validWidget(-1);
	}
}

} // namespace Status end
} // namespace BMW end