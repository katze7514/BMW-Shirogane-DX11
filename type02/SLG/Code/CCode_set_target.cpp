/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_set_target.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_set_target::OnAction(Task::CTaskContext* pContext)
{// ‘ÎÛ‚ðÝ’è‚·‚é
	pContext->setValue(pContext->top(), getState()==CHARA ? Flag::TARGET_CHARA : Flag::TARGET_MAP);
	pContext->pop();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
