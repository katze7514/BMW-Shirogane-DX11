#include "stdafx.h"

#include "../../Scene/IScene.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "CVictory_change.h"

namespace BMW{
namespace SLG{
namespace Victory{

void CVictory_change::OnReset(Task::CTaskContext* pContext)
{
	//BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	//addTask(pOK,OK);
	//pOK->setValue(CLICK);

	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_ANNOUNCE");
	addTask(pPanel_,BACK);
	pText_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("CHANGE");
}

void CVictory_change::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pContext->getInput()->guard(false);

	nType_=pContext->top();
	pContext->pop();

#ifdef BMW_DEBUG
	CDbg().Out("VIC_TYPE %d",nType_);
#endif

	if(nType_==EXPERT_GET)
	{// 熟練度獲得
		pText_->validWidget("EXPERT_GET");
		pContext->setValue(-1,Flag::EXPERT_C);
	}
	else
	{// 勝利条件関係
	 //	だったら、スタックに勝利条件フラグ、敗北条件フラグ、熟練度条件フラグと積まれている
		pText_->validWidget("VICTORY");
		bool bVic[3]={false,false,false};
		// 勝利条件フラグ
		if(pContext->top()>-3)
		{// 勝利条件変更！
			bVic[0]=true;
			pContext->setValue(pContext->top(),Flag::VICTORY);
		}
		pContext->pop();

		// 敗北条件フラグ
		if(pContext->top()>-3)
		{// 敗北条件変更！
			bVic[1]=true;
			pContext->setValue(pContext->top(),Flag::LOSE);
		}
		pContext->pop();

		// 熟練度条件フラグ
		if(pContext->getValue(Flag::EXPERT_C)!=-1
		&& pContext->top()>-3)
		{// 熟練度変更！
			bVic[2]=true;
			pContext->setValue(pContext->top(),Flag::EXPERT_C);
		}
		pContext->pop();

		// 表示フラグ
		bool bApper = pContext->top()==0 ? false : true;
		pContext->pop();

		// パネルセットアップ！
		if(bApper && (bVic[0]||bVic[1]||bVic[2]))
		{	updatePanel(bVic);	}
		else
		{// 全部falseだったらそのままリターン
			pContext->getInput()->guard(true);
			setState(END);
		}
	}
}

void CVictory_change::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::OK)==Input::IInput::RELEASE
		|| pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// 何か押されたら次へ
			pContext->getInput()->guard(true);
			if(nType_==EXPERT_GET)
			{	setState(END);	}
			else
			{
				getTaskListCtrl()->callTaskList(Rule::VICTORY_VIEW,true);
				setState(NORMAL);
			}
		}
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CVictory_change::OnComeBack(int nID, Task::CTaskContext*)
{
	setState(END);
}

void CVictory_change::updatePanel(bool bVic[])
{
	GUI::CPanel* pPanel = pText_->getWidgetCast<GUI::CPanel>("VICTORY");
	Task::ITaskBase* pRow[3]={NULL,NULL,NULL};
	// まずは、表示する行を取得
	int nRow=0;
	for(int i=0; i<3; ++i)
	{
		if(bVic[i])
		{// 変更対象！
			pRow[nRow++]=pPanel->getWidget(i);
			pRow[nRow-1]->visible(true);
		}
		else
		{// そうじゃない 
			pPanel->getWidget(i)->visible(false);
		}
	}

	// 順番に位置設定
	switch(nRow)
	{
	case 1: // 1行
		pRow[0]->setY(0);
	break;

	case 2: // 2行
		pRow[0]->setY(-11);
		pRow[1]->setY(11);
	break;

	default: // 3行
		pRow[0]->setY(-22);
		pRow[1]->setY(0);
		pRow[2]->setY(22);
	break;
	}
}

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end