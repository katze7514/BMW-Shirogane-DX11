#include "stdafx.h"

#include "../IDRule.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CMove_rule_cpu.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_rule_cpu::OnInit(Task::CTaskContext* pContext)
{
	nWait_=0;
	// ‚Ü‚¸‚ÍAˆÚ“®”ÍˆÍ•\Ž¦
	Map::CMapChipState::move(true);
	setState(VIEW);
}

void CMove_rule_cpu::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case VIEW:
		// ‚¿‚å‚Á‚ÆˆÚ“®”ÍˆÍ‚ðŒ©‚¹‚Ä‚¨‚­
		if(nWait_++>=20) setState(ROAD);
	break;

	case ROAD:
		getTaskListCtrl()->callTaskList(Rule::MOVE_ROAD, true);
	break;

	case EXEC:
		getTaskListCtrl()->callTaskList(Rule::MOVE_EXEC, true);
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CMove_rule_cpu::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::MOVE_ROAD:  Map::CMapChipState::move(false); setState(EXEC);	break;
	case Rule::MOVE_EXEC: setState(END);	break;
	}
}

} // namespace BMW end
} // namesapce SLG end
} // namesapace Move end