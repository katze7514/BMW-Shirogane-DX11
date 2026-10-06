#include "stdafx.h"

#include "CRuleMenuButton.h"
#include "CRuleButton.h"

namespace BMW{
namespace Rule{

void CRuleButton::actionRelease(Task::CTaskContext* pContext)
{
	CButtonGraphic::actionRelease(pContext);
	getParent()->setState(CRuleMenuButton::RELEASE);
}

} // namespace Rule end
} // namespace BMW end