/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_cursol_pos.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_cursol_pos::OnAction(Task::CTaskContext* pContext)
{// カーソル位置変更
	pContext->getInput()->setCursolPos(getX(), getY());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
