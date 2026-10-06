#include "stdafx.h"

#include "../IDSLG.h"

#include "CPhase_base.h"

namespace BMW{
namespace SLG{
namespace Phase{

void CPhase_base::actionPhase(Task::CTaskContext* pContext)
{
}

bool CPhase_base::IsTurn(int nTurn, Task::CTaskContext& context)
{
	return nTurn==context.getValue(Flag::TURN);
}

bool CPhase_base::IsPhase(int nPhase, Task::CTaskContext& context)
{
	return nPhase==context.getValue(Flag::PHASE);
}

bool CPhase_base::IsTurnPhase(int nTurn, int nPhase, Task::CTaskContext& context)
{
	return IsTurn(nTurn,context) && IsPhase(nPhase,context);
}

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end