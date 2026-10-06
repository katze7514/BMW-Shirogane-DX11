#include "stdafx.h"

#include "CCode_stop.h"

namespace BMW{
namespace Movie{
namespace Code{

void CCode_stop::OnAction(Task::CTaskContext* pContext)
{
	// ƒtƒŒ[ƒ€‚ðŽ~‚ß‚Ä‚µ‚Ü‚¤
	pContext->getTaskList()->getParent()->setState(-1);
	// ó‹µ‚Íi‚ß‚é
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace Movie end
} // namespace BMW end