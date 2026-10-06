/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_input_enable.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_input_enable::OnAction(Task::CTaskContext* pContext)
{// “ü—Í‚ÌƒK[ƒh
	pContext->getInput()->guard(getState()==0);
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
