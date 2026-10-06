/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_pop.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_pop::OnAction(Task::CTaskContext* pContext)
{// スタックをpopする
	pContext->pop();
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
