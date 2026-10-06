/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_ret.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_ret::OnAction(Task::CTaskContext* pContext)
{// サブルーチンをリターンする
	pContext->getTaskList()->getTaskListCtrl()->returnTaskList();
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
