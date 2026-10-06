/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_halt.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_halt::OnAction(Task::CTaskContext* pContext)
{// ƒQ[ƒ€‚ÌI—¹
	pContext->getApp()->end();
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
