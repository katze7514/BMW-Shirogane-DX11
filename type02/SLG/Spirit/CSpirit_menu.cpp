#include "stdafx.h"

#include "../../Spirit/IDSpirit.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CGage.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDSLG.h"
#include "../slg_fun.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Map/CMap.h"

#include "CSpirit_menu.h"

namespace BMW{
namespace SLG{
namespace Spirit{
////////////////////////////////////////////////
// タスク
////////////////////////////////////////////////
void CSpirit_menu::OnReset(Task::CTaskContext* pContext)
{
	// CANCEL_T
	Rule::CRuleCancel* pCancel = new Rule::CRuleCancel(CANCEL);
	addTask(pCancel,CANCEL_T);

	// インターフェイスを取得
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// 精神サークルメニュー
	pMenu_ = db.createInterfaceCast<GUI::CCircleMenu>("SPIRIT_CIRCLE");
	addTask(pMenu_,MENU);
	// イベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CSpirit_menu::eventCircle);
	pMenu_->setEventHandler(funCircle);

	// SP現在値
	pSp_ = db.createInterfaceCast<GUI::CPanel>("SP_MAP");
	addTask(pSp_,SP);
	pSpGage_ = pSp_->getWidgetCast<GUI::CGage>("GAGE");
}

void CSpirit_menu::OnInit(Task::CTaskContext* pContext)
{
	// 精神コマンド設定
	// コンテキスト設定
	CSLGContext*	p = static_cast<CSLGContext*>(pContext);

	// 表示座標設定
	//p->getInput()->setCursolPos(p->getCircleX(),p->getCircleY());
	// 座標位置自体は、メニュー選択時に行われている
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());
	pSp_->setX(p->getCircleX());
	pSp_->setY(p->getCircleY());

	// キャラデータ
	CDataCharaSLG*	pChara	= p->getCtrlCharaData();
	p->getMap()->scrollIndex(pChara->getIndex());

	// とりあえず、リセット
	pMenu_->resetButton();

	// 精神を設定
	GUI::CPanel*		pPanel;
	GUI::CButtonSymbol*	pIcon;
	GUI::CNum*			pSp;
	bool				bEnable;
	string				sID;

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CSpirit_menu::eventButton);

	BMW::Spirit::CSpiritDB& sDB = p->getApp()->getSpirit();

	// 集中力を持っている？
	bool bConcent = pChara->getBattle().hasSkill(Ability::CONCENT)!=-1;

	for(int i=1; i<=6; i++)
	{
		const Chara::CStatusAbility& spirit = pChara->getBattle().getSpirit(i-1);

		if(spirit.getID()>=0)
		{// 有効な精神IDだったら
			sID = "SPIRIT" + CStringScanner::NumToString(i);
			pPanel = pMenu_->getButton(sID)->getTaskCast<GUI::CPanel>();
			pIcon = pPanel->getWidgetCast<GUI::CButtonSymbol>("ICON");
			// 精神は使える？
			bEnable = sDB.enable(*pChara,spirit.getAttr(),*p,spirit.getID());
			// 精神アイコン
			sDB.setButtonHolder(pIcon, spirit.getID(), funButton, i-1);
			pIcon->valid(bEnable);
			// SP
			pSp = pPanel->getWidgetCast<GUI::CNum>("SP_SPEND");
			pSp->setNum(bConcent
						? ((spirit.getAttr()*4)/5<=0 
							? 1
							: (spirit.getAttr()*4)/5
						  )
						: spirit.getAttr());
			// ぺけ
			pPanel->getWidget("PEKE")->visible(!bEnable);
			// 最後に有効化
			pMenu_->validButton(true, sID, p);
		}
	}

	// 登場セット
	actionMenu(GUI::CCircleMenu::INTRO,p);

	// SP値設定
	pSpGage_->actionChangeNum(pChara->getBattle().getSP(),pChara->getBattle().getMaxSP());
}

void CSpirit_menu::OnAction(Task::CTaskContext* pContext)
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
void CSpirit_menu::actionMenu(int nState, Task::CTaskContext* pContext)
{
	IListenerCircleMenu::actionMenu(nState,pContext);
	setState(NORMAL);
}

////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////
void CSpirit_menu::eventCircle(int nState, Task::CTaskContext* pContext)
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
		// ヘルプモード
		Unit::Help::callHelp(Unit::Help::SLG_SPIRIT, "SLG_SPIRIT", pContext);
	}
}

void CSpirit_menu::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// ボタンが押されたらTARGET_ABILITYを設定
		pContext->setValue(pButton->getValue(), Flag::TARGET_ABILITY);
		// ボタン押された！
		setState(OK);
	}
}

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end