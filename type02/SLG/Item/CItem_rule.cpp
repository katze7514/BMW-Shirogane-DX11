#include "stdafx.h"

#include "../IDRule.h"
#include "../Demo/CDemo_map.h"

#include "CItem_rule.h"

namespace BMW{
namespace SLG{
namespace Item{

void CItem_rule::OnReset(Task::CTaskContext* pContext)
{
}

void CItem_rule::OnInit(Task::CTaskContext* pContext)
{
	setState(MENU);
}

void CItem_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case MENU:	getTaskListCtrl()->callTaskList(Rule::ITEM_MENU,true);	break;

	case EFFECT:
		// アイテムエフェクト表示
		pContext->push(Demo::CDemo_map::ITEM);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	break;

	case APPLY:	getTaskListCtrl()->callTaskList(Rule::ITEM_APPLY,true);	break;

	case END:	
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(true);
	break;

	default: break;
	}
}

void CItem_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::ITEM_MENU:
		if(pContext->top()==-1)
		{// -1だったらキャンセルされたので、キャラメニューへ
			setState(END);
		}
		else
		{// そうじゃなければ、対象キャラ選択へ
			setState(EFFECT);
			pContext->pop();
		}
	break;

	case Rule::DEMO_MAP:
		// エフェクトが終わったら、適用へ
		setState(APPLY);
	break;

	case Rule::ITEM_APPLY:
		// 適用が終わったら終了
		pContext->push(0);
		setState(END);
	break;

	default: break;
	}
}

} // namespace Item end
} // namespace SLG end
} // namespace BMW end