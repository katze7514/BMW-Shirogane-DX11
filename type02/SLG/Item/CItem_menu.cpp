#include "stdafx.h"

#include "../../Item/IDItem.h"
#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../slg_fun.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CItem_menu.h"

namespace BMW{
namespace SLG{
namespace Item{

void CItem_menu::OnReset(Task::CTaskContext* pContext)
{
	// CANCEL_T
	Rule::CRuleCancel* pCancel = new Rule::CRuleCancel();
	addTask(pCancel,CANCEL_T);
	pCancel->setValue(CANCEL);

	pMenu_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CCircleMenu>("ITEM_CIRCLE");
	addTask(pMenu_,MENU);
	// イベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CItem_menu::eventCircle);
	pMenu_->setEventHandler(funCircle);
}

void CItem_menu::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 表示座標設定
	//p->getInput()->setCursolPos(p->getCircleX(),p->getCircleY());
	// 座標位置自体は、メニュー選択時に行われている
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());

	CDataCharaSLG*	pChara	= p->getCtrlCharaData();

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CItem_menu::eventButton);

	int nPos=1;
	int nID;
	string sID;
	BMW::Item::CItemDB& iDB = p->getApp()->getItem();
	for(int i=0; i<pChara->getBattle().getItemMax(); i++)
	{// アイテムの個数分
		nID = pChara->getBattle().hasItemAttr(i);
		if(nID>=0
		&& iDB.IsUse(nID)
		&& iDB.enable(*pChara,i,*p,nID))
		{// 消費アイテムのみ表示
			sID = Misc::linkStrAndNum("ITEM",nPos++);
			iDB.setButtonHolder(pMenu_->getButton(sID)->getTaskCast<GUI::CButtonSymbol>(),nID,funButton,i);
			pMenu_->validButton(true,sID,pContext);
		}
	}
	// 登場セット
	actionMenu(GUI::CCircleMenu::INTRO,p);

	setState(NORMAL);

	// ヘルプモード
	//Unit::Help::callHelp(Unit::Help::SLG_ITEM, "SLG_ITEM", pContext);
}

void CItem_menu::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
		// キャンセルしたら、-1を積んでリターン
		pContext->push(-1);
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
	break;

	case OK:
		// 決定されたので、0を積んでリターン
		pContext->push(0);
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
	break;

	default: break;
	}
}

////////////////////////////////////////////////
// アクション
////////////////////////////////////////////////
void CItem_menu::actionMenu(int nState, Task::CTaskContext* pContext)
{
	IListenerCircleMenu::actionMenu(nState,pContext);
	setState(NORMAL);
}

////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////
void CItem_menu::eventCircle(int nState, Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::EXIT)
	{// 動作終了したら、リターン
		// カーソル位置をずらしておく
		moveCursol(pContext);
		getTaskListCtrl()->returnTaskList();
	}
	else
	{// 登場動作終了
		pContext->getInput()->guard(false);
	}
}

void CItem_menu::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// ボタンが押されたらTARGET_ABILITYを設定
		pContext->setValue(pButton->getValue(), Flag::TARGET_ABILITY);
		// ボタン押された！
		setState(OK);
	}
}


} // namespace Item end
} // namespace SLG end
} // namespace BMW end