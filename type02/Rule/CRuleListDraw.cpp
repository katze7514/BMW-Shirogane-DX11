#include "stdafx.h"

#include "CRuleListDraw.h"

namespace BMW{
namespace Rule{

void CRuleListDraw::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{// êeÇ™å„îªíË
		if(IsValid())
		{
			callTaskAction(pContext);
			OnAction(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			callTaskDraw(pContext);
		}
	}
}

} // namespace Task end
} // namespace BMW end