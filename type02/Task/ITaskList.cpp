#include "stdafx.h"

#include "CTaskContext.h"
#include "ITaskList.h"

namespace BMW{
namespace Task{

void ITaskList::Task(CTaskContext* pContext)
{//	e‚ÌValid/Visible‚ÉŽq‚à‰e‹¿‚³‚ê‚é
	if(pContext->IsAction())
	{
		if(IsValid()){
			OnAction(pContext);
			callTaskAction(pContext);
		}
	}
	else
	{
		if(IsVisible()){
			callTaskDraw(pContext);
		}
	}
}

} // namespace Task end
} // namespace BMW end