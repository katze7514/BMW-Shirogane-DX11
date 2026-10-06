/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_jump.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_jump::OnAction(Task::CTaskContext* pContext)
{// –³ðŒƒWƒƒƒ“ƒv
	pContext->getTaskList()->setState(getState());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
