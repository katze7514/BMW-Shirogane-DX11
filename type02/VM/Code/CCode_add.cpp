/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_add.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_add::OnAction(Task::CTaskContext* pContext)
{// ‰ÁŽZ
	int lhs = pContext->top();
	pContext->pop();
	int rhs = pContext->top();
	pContext->pop();
	pContext->push(rhs + lhs);
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
