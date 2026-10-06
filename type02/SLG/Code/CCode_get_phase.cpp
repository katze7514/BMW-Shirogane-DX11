/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_get_phase.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_get_phase::OnAction(Task::CTaskContext* pContext)
{// フェーズを取得する
	pContext->push(pContext->getValue(Flag::PHASE));
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
