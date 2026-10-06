/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_sub.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_sub::OnAction(Task::CTaskContext* pContext)
{// Œ¸ŽZ
	int lhs = pContext->top();
	pContext->pop();
	int rhs = pContext->top();
	pContext->pop();
	pContext->push(rhs - lhs);
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
