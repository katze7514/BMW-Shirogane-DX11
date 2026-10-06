#include "stdafx.h"

#include "../IDRule.h"
#include "../Demo/CDemo_map.h"
#include "CSpirit_rule.h"

namespace BMW{
namespace SLG{
namespace Spirit{

void CSpirit_rule::OnReset(Task::CTaskContext* pContext)
{
}

void CSpirit_rule::OnInit(Task::CTaskContext* pContext)
{
	setState(MENU);
}

void CSpirit_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case MENU:	getTaskListCtrl()->callTaskList(Rule::SPIRIT_MENU,true);		break;
	case SELECT:getTaskListCtrl()->callTaskList(Rule::SPIRIT_SELECT,true);		break;

	case EFFECT:
		// 精神エフェクト表示
		pContext->push(Demo::CDemo_map::SPIRIT);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	break;

	case APPLY:	getTaskListCtrl()->callTaskList(Rule::SPIRIT_APPLY,true);		break;

	case END:	
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(false);
	break;

	default: break;
	}
}

void CSpirit_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::SPIRIT_MENU:
		if(pContext->top()==-1)
		{// -1だったらキャンセルされたので、キャラメニューへ
			setState(END);
		}
		else
		{// そうじゃなければ、対象キャラ選択へ
			setState(SELECT);
			pContext->pop();
		}
	break;

	case Rule::SPIRIT_SELECT:
		if(pContext->top()==-1)
		{// -1だったらキャンセルされたので、もう一度精神選択へ
			setState(MENU);
		}
		else
		{// そうじゃなければ、EFFECTへ
			setState(EFFECT);
		}
		pContext->pop();
	break;

	case Rule::DEMO_MAP:
		// エフェクトが終わったら、適用へ
		setState(APPLY);
	break;

	case Rule::SPIRIT_APPLY:
		// 適用が終わったら終了
		pContext->push(0); 
		setState(END);
	break;

	default: break;
	}
}

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end