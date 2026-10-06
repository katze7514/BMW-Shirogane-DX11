/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_ipush.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_ipush::OnAction(Task::CTaskContext* pContext)
{// ”’l‚Ìpush
	pContext->push(getState());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
