/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_nop.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_nop::OnAction(Task::CTaskContext* pContext)
{// ƒtƒŒ[ƒ€‚ði‚ß‚é
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
