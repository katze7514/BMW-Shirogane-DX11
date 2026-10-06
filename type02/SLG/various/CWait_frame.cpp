#include "stdafx.h"

#include "CWait_frame.h"

namespace BMW{
namespace SLG{

void CWait_frame::OnInit(Task::CTaskContext* pContext)
{
	// スタックトップにウェイトフレームが入ってる
	setState(pContext->top());
	pContext->pop();
}

void CWait_frame::OnAction(Task::CTaskContext* pContext)
{
	if(--nState_<=0)
		getTaskListCtrl()->returnTaskList();
}

} // namespace SLG end
} // namespace BMW end