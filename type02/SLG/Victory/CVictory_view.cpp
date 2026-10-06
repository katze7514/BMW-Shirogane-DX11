#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../IDSLG.h"

#include "CVictory_view.h"

namespace BMW{
namespace SLG{
namespace Victory{

namespace{
__inline void initVicPanel(GUI::CPanelCtrl* pPanel)
{
	GUI::CText* pText = new GUI::CText();
	pText->setFont(GUI::CText::FONT_GOTHIC);
	pText->setText("・？？？");
	pText->UpdateTextAA();
	pPanel->addWidget(pText, "INVISIBLE");

	pText = new GUI::CText();
	pText->setFont(GUI::CText::FONT_GOTHIC);
	pText->setText("・なし");
	pText->UpdateTextAA();
	pPanel->addWidget(pText, "INVALID");
}
} // namespace end

void CVictory_view::OnReset(Task::CTaskContext* pContext)
{// 受け入れだけ作る感じ
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK(CLICK);
	addTask(pOK,OK);
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel(CLICK);
	addTask(pCancel,CANCEL);

	GUI::CPanel* pPanel = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_VICTORY");
	addTask(pPanel,BACK);
	pVictory_	= pPanel->getWidgetCast<GUI::CPanelCtrl>("VICTORY");
	initVicPanel(pVictory_);
	pLose_		= pPanel->getWidgetCast<GUI::CPanelCtrl>("LOSE");
	initVicPanel(pLose_);
	pExpert_	= pPanel->getWidgetCast<GUI::CPanelCtrl>("EXPERT");
	initVicPanel(pExpert_);

	// 話とかタイトルとか取得設定
	pPanel = pPanel->getWidgetCast<GUI::CPanel>("TITLE");
	Scenario::CScenarioDB& db = pContext->getApp()->getScenario();
	int nStory = pContext->getApp()->getExec().getStory(); 
	pPanel->getWidgetCast<GUI::INum>("CAPTER")->setNum(db.getNo(nStory));
	//TITLE
	GUI::CText* pText = pPanel->getWidgetCast<GUI::CText>("STORYTITLE");
	pText->setText(db.getTitle(nStory));
	pText->getFontConf().SetWeight(700);
	pText->UpdateTextAA();
}

namespace{
__inline void updateVicPanel(GUI::CPanelCtrl* pPanel, int n)
{
	if(n==Expert::INVISIBLE)
		pPanel->validWidget("INVISIBLE");
	ef(n==Expert::INVALID)
		pPanel->validWidget("INVALID");
	else
		pPanel->validWidget(n);
}
} // namespace end

void CVictory_view::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pContext->getInput()->guard(false);

	// 各条件表示
	updateVicPanel(pVictory_,pContext->getValue(Flag::VICTORY));
	updateVicPanel(pLose_,pContext->getValue(Flag::LOSE));
	updateVicPanel(pExpert_,pContext->getValue(Flag::EXPERT_C));
}

void CVictory_view::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CLICK:
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(true);
	break;

	default: break;
	}
}

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end