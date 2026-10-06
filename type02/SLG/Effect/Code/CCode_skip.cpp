#include "stdafx.h"

#include "CCode_skip.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_skip::OnAction(Task::CTaskContext* pContext)
{
	if(pContext->top()<0)
		pContext->getApp()->animeSkip();
	else
		pContext->getApp()->skip(pContext->top());

	pContext->pop();
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end