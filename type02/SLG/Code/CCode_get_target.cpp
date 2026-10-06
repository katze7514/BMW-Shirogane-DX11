/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "CCode_get_target.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_get_target::OnAction(Task::CTaskContext* pContext)
{// ターゲット取得
	pContext->push(pContext->getValue(getState()==CHARA ? Flag::TARGET_CHARA : Flag::TARGET_MAP));
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
