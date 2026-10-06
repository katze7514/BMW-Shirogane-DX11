#include "stdafx.h"

#include "CCode_valid.h"

namespace BMW{
namespace ADV{
namespace Code{

void CCode_valid::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==CLEAR)
	{
	#ifdef BMW_DEBUG
		CDbg().Out("VALID CLEAR");
	#endif
		pContext->getApp()->getExec().getValidSet().clear();
		return;
	}

	while(pContext->top()>=0)
	{
	#ifdef BMW_DEBUG
		CDbg().Out("VALID %d", pContext->top());
	#endif
		if(getState()==ADD){
			pContext->getApp()->getExec().addValid(pContext->top());
		}
		else{
			pContext->getApp()->getExec().delValid(pContext->top());
		}

		pContext->pop();
	}
	pContext->pop();
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end