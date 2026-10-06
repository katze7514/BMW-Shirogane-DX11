/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_get_value.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_get_value::OnAction(Task::CTaskContext* pContext)
{// ˆêŽžƒf[ƒ^‚ÌŽæ“¾
	pContext->push(pContext->getValue(getState()));
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
