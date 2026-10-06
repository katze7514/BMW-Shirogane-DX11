#include "stdafx.h"

#include "../CADVContext.h"
#include "../CMsgBoard.h"

#include "CCode_msg_state.h"

namespace BMW{
namespace ADV{
namespace Code{

void CCode_msg_state::OnAction(Task::CTaskContext* pContext)
{
	// 操作サイド
	int nSide = pContext->top();
	pContext->pop();

	CADVContext* p = static_cast<CADVContext*>(pContext);
	// 変更するメッセージボードを取得
	smart_ptr<CMsgBoard>& pMsg = p->getMsgBoard(nSide);
	switch(getState())
	{
	case VALID:
		pMsg->msgValid(true);
	break;
	case INVALID:
		pMsg->msgValid(false);
	break;
	case VISIBLE:
		pMsg->visible(IsVisible());
	break;
	default: break;
	}

	// 反映するためにフレームを回す
	pContext->getTaskList()->killMe();

}

} // namespace Code end
} // namespace ADV end
} // namesapce BMW end