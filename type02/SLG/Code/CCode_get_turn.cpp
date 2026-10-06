/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_get_turn.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_get_turn::OnAction(Task::CTaskContext* pContext)
{// ƒ^[ƒ“”‚ðŽæ“¾‚·‚é
	pContext->push(pContext->getValue(Flag::TURN));
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
