#include "stdafx.h"

#include "../../ADV/CMsgBoard.h"

#include "../Context/CSLGContext.h"

#include "CEvent.h"
#include "CEvent_Msg_state.h"

namespace BMW{
namespace SLG{
namespace Event{


void CEvent_Msg_state::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->getEvent()->validBoard(IsVisible());
	// ”½‰f‚·‚é‚½‚ß‚ÉƒtƒŒ[ƒ€‚ð‰ñ‚·
	p->getTaskList()->killMe();
}

} // namespace Event end
} // namespace SLG end
} // namespace BMW end