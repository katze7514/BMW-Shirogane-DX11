/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_call.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_call::OnAction(Task::CTaskContext* pContext)
{// サブルーチン呼び出し
	pContext->getTaskList()->getTaskListCtrl()->callTaskList(getState(),true);
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
