#include "stdafx.h"

#include "CCode_parent_state.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_parent_state::OnAction(Task::CTaskContext* pContext)
{
	pContext->getTaskList()->getParent()->setState(getState());
	// Œ‹‰Ê‚ð”½‰f‚³‚¹‚é‚½‚ß‚ÉƒtƒŒ[ƒ€‚ð‰ñ‚·
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end