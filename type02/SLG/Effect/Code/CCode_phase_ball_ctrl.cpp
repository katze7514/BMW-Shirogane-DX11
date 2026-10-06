#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Event/CEvent.h"
#include "../../Phase/CPhaseBall.h"

#include "CCode_phase_ball_ctrl.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_phase_ball_ctrl::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	p->getEvent()->getTurnBall().intro(p->top());
	p->pop();
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end