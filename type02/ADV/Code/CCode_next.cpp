#include "stdafx.h"

#include "CCode_next.h"

namespace BMW{
namespace ADV{
namespace Code{

void CCode_next::OnAction(Task::CTaskContext* pContext)
{
	// ŽŸ‚ÌƒVƒiƒŠƒI‚ðÝ’è
	pContext->getApp()->getExec().setNextScenario(getState());

#ifdef BMW_DEBUG
	CDbg().Out("NEXT %d", getState());
#endif
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end