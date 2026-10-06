/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_get_ctrl_chara.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_get_ctrl_chara::OnAction(Task::CTaskContext* pContext)
{// ‘€ì‘ÎÛƒLƒƒƒ‰‚ÌŽæ“¾
	pContext->push(pContext->getValue(Flag::CTRL_CHARA));
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
