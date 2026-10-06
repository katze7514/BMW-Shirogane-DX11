#include "stdafx.h"

#include "CRuleMenuButton.h"

namespace BMW{
namespace Rule{

void CRuleMenuButton::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case RELEASE: getParent()->setState(getValue()); setState(NORMAL); break;
	default: break;
	}
}

} // namespace Rule end
} // namespace BMW end