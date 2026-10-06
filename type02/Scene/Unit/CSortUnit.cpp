#include "stdafx.h"

#include "../IScene.h"
#include "../GUI/CCircleMenu.h"
#include "../GUI/CCircleMenuButton.h"

#include "CSortUnit.h"

namespace BMW{
namespace Unit{

CSortUnit::~CSortUnit()
{
	DELETE_SAFE(pCancel_);
	DELETE_SAFE(pMenu_);
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(pOrderMovie_[0]);
	DELETE_SAFE(pOrderMovie_[1]);
}

void CSortUnit::Task(Task::CTaskContext* pContext)
{
	pCancel_->Task(pContext);
	pOrderMovie_[nOrder_]->Task(pContext);
	pPanel_->Task(pContext);
	pMenu_->Task(pContext);
	if(pContext->IsAction() && IsValid()) OnAction(pContext);
}

void CSortUnit::OnInit(Task::CTaskContext* pContext)
{
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();
	// サークルメニュー取得
	pMenu_ = db.createInterfaceCast<GUI::CCircleMenu>("SORT_CIRCLE");
	pMenu_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pMenu_->valid(false);
	pMenu_->visible(false);
	// イベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this, &CSortUnit::eventCircle);
	pMenu_->setEventHandler(funCircle);

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CSortUnit::eventButton);
	pMenu_->setButtonEventHandler("ID",funButton,ID);
	pMenu_->setButtonEventHandler("LV",funButton,LV);
	pMenu_->setButtonEventHandler("HP",funButton,HP);
	pMenu_->setButtonEventHandler("EN",funButton,EN);
	pMenu_->setButtonEventHandler("SP",funButton,SP);
	pMenu_->setButtonEventHandler("KI",funButton,KI);
	pMenu_->setButtonEventHandler("NEXT",funButton,NEXT);

	pCancel_ = new Rule::CRuleCancel(CANCEL);
	pCancel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pCancel_->valid(false);

	// パネル取得
	pPanel_ = db.createInterfaceCast<GUI::CPanel>("SORTUNIT");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pKeyButton_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("SORT");
	// イベントハンドラ設定
	funButton.set(this,&CSortUnit::eventSort);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("ID"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("LV"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("HP"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("EN"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("SP"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("KI"),funButton,KEY);
	GUI::CButton::setButtonEvent(pKeyButton_->getWidgetCast<GUI::CButton>("NEXT"),funButton,KEY);

	pKeyDisp_ = pPanel_->getWidgetCast<GUI::CText>("SORT_DISPLAY");
	// イベントハンドラ設定
	pArrow_ = pPanel_->getWidgetCast<GUI::CButton>("ORDER_BUTTON");
	GUI::CButton::setButtonEvent(pArrow_, funButton, ORDER);
	pArrow_->visible(false);
	// 動き取得
	pOrderMovie_[UP] = db.getSymbolDB().createSymbolStrCast<Movie::CMovieClip>("SYOUKOU_ROTATE_R");
	pOrderMovie_[UP]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pOrderMovie_[DOWN] = db.getSymbolDB().createSymbolStrCast<Movie::CMovieClip>("SYOUKOU_ROTATE");
	pOrderMovie_[DOWN]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pOrderDisp_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("ORDER_DISPLAY");

	// ID表示、昇順がデフォルト
	nKey_=ID;
	nOrder_=UP;
	pOrderMovie_[UP]->OnReset(pContext);
	bool b = pContext->IsAction();
	pContext->action(true);
	while(!pOrderMovie_[UP]->IsStop()) pOrderMovie_[UP]->Task(pContext);
	pContext->action(b);
	actionSort();
}

void CSortUnit::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case WAIT:
		if(pOrderMovie_[nOrder_]->IsStop())
		{//	動作が終了したら止まる
			//pOrderMovie_[nOrder_]->valid(false);
			actionSort();
			pArrow_->valid(true);
			pContext->getInput()->guard(false);
			// 親動作もどし
			fun_(END,0,pContext);
			setState(NORMAL);
		}
	break;

	case CANCEL:
		pCancel_->valid(false);
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
		// 親動作をもどし
		fun_(END,0,pContext);
		setState(NORMAL);
	break;

	default: break;
	}
}

/////////////////////////////////////////////////////////
// アクション
/////////////////////////////////////////////////////////
namespace{
const string sKeyName[]=
{
	"ID",
	"LV",
	"HP",
	"EN",
	"SP",
	"気力",
	"Next"
};
} // namespace end
void CSortUnit::actionSort()
{// 現在の状態に合わせて、表示を変更する
	// ソートキー
	pKeyButton_->validWidget(nKey_);
	// Key表示
	pKeyDisp_->setText(sKeyName[nKey_]);
	pKeyDisp_->UpdateTextAA();

	// Order
	//pOrderMovie_[nOrder_]->valid(false);
	// Orderテキスト
	pOrderDisp_->validWidget(nOrder_==UP?"SYOUJUN":"KOUJUN");
}

void CSortUnit::actionKey(Task::CTaskContext* pContext)
{
	if(getKey()!=ID)	pMenu_->validButton(true,"ID",pContext);
	if(getKey()!=LV)	pMenu_->validButton(true,"LV",pContext);
	if(getKey()!=HP)	pMenu_->validButton(true,"HP",pContext);
	if(getKey()!=EN)	pMenu_->validButton(true,"EN",pContext);
	if(getKey()!=SP)	pMenu_->validButton(true,"SP",pContext);
	if(getKey()!=KI)	pMenu_->validButton(true,"KI",pContext);
	if(getKey()!=NEXT)	pMenu_->validButton(true,"NEXT",pContext);
}

/////////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////////
void CSortUnit::eventSort(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 何か押されたあるよ
		pContext->getInput()->guard(true);
		if(pButton->getValue()==KEY)
		{// ソートキー選択サークルメニュー表示
			actionKey(pContext);
			actionMenu(GUI::CCircleMenu::INTRO,pContext);
			pMenu_->valid(true);
			pMenu_->visible(true);
			pKeyButton_->valid(false);

			// これとOrderは同時に反応するので、Order動作を止める
			pArrow_->valid(false);
			pOrderMovie_[nOrder_]->valid(false);
			setState(NORMAL);

			// 親動作を止める
			fun_(START,0,pContext);
		}
		else
		{// 降順・昇順入れ替え
			nOrder_=!nOrder_;
			pOrderMovie_[nOrder_]->OnReset(pContext);
			pOrderMovie_[nOrder_]->valid(true);
			pArrow_->valid(false);
			setState(WAIT);
			// 親動作を止める
			fun_(START,0,pContext);
			// 変更を通知
			fun_(nKey_,nOrder_,pContext);
		}
	}
}

void CSortUnit::eventCircle(int nState, Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::INTRO)
	{ 
		pCancel_->valid(true); 
	}
	else
	{ 
		pMenu_->valid(false); 
		pMenu_->visible(false); 
		pKeyButton_->valid(true); 
		pArrow_->valid(true);
		// 親動作をもどし
		fun_(END,0,pContext);
		actionSort();
	}
	pContext->getInput()->guard(false);
}

void CSortUnit::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 何か押されたあるよ
		pCancel_->valid(false);
		nKey_=pButton->getValue();
		actionMenu(GUI::CCircleMenu::EXIT,pContext);

		// 変更を通知
		fun_(nKey_,nOrder_,pContext);
	}
}

} // namepsace Unit end
} // namespace BMW end