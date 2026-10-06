#include "stdafx.h"

#include "../IDRule.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CLove_rule.h"

namespace BMW{
namespace SLG{
namespace Love{

void CLove_rule::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
}

void CLove_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END:
		pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
		getTaskListCtrl()->returnTaskList();
	break;

	case WAIT:
		getTaskListCtrl()->callTaskList(Rule::WAIT_RULE,true);
	break;

	default:
	{
		// 告白イベントを呼ぶ
		pContext->getInput()->cursolVisible(false);
		getTaskListCtrl()->callTaskList(Rule::LOVE_START,true);
		// 告白リストをポップする
		static_cast<CSLGContext*>(pContext)->getCtrlCharaData()->popPers();
	}
	break;
	}
}

void CLove_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::LOVE_START)
	{// 告白が終わったら、WAIT
		setState(WAIT);
	}
	else
	{// 終了
		setState(END);
	}
}

} // namespace Love end
} // namespace SLG end
} // namespace BMW end