#include "stdafx.h"

#include "../../Face/DB/CFaceDB.h"
#include "../CDemoContext.h"
#include "../GUI/CDemoMsgBoard.h"
#include "CCode_msg_Demo.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_msg_demo::OnAction(Task::CTaskContext* pContext)
{
	// ìoò^Ç≥ÇÍÇΩMSGÇ…ïœçXÇ∑ÇÈ
	CDemoContext* p = static_cast<CDemoContext*>(pContext);
	p->getMsgBoard()->changeMsg(getState(),pContext);
}

} // namespace Code end
} // namespace Demo end
} // namespace BMW end