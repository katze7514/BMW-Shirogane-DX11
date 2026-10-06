#include "stdafx.h"

#include "CCode_help.h"

namespace BMW{
namespace Scene{
namespace Code{

void CCode_help::OnAction(Task::CTaskContext* pContext)
{
	// ヘルプID
	int nID = pContext->top();
	pContext->pop();
	// ヘルプ出現！
	pContext->getApp()->getFoward()->createHelp(nID, pContext);
	// フレーム進める
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace Scene end
} // namespace BMW end