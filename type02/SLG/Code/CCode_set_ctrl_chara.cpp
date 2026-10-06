/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_set_ctrl_chara.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_set_ctrl_chara::OnAction(Task::CTaskContext* pContext)
{// ‘€ì‘ÎÛƒLƒƒƒ‰‚ðÝ’è‚·‚é
	pContext->setValue(pContext->top(), Flag::CTRL_CHARA);
	pContext->pop();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
