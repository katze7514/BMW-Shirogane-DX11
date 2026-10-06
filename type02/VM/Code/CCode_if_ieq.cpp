/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_if_ieq.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_if_ieq::OnAction(Task::CTaskContext* pContext)
{// ==‚©‚Á‚½‚çAÝ’è‚³‚ê‚Ä‚éƒ‰ƒxƒ‹‚É”ò‚Ô
	int lhs = pContext->top();
	pContext->pop();
	int rhs = pContext->top();
	pContext->pop();
	if(rhs == lhs) pContext->getTaskList()->setState(getState());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
