#include "stdafx.h"

#include "CChange_chara.h"

namespace BMW{
namespace ADV{
namespace Code{

void CChange_chara::OnAction(Task::CTaskContext* pContext)
{
	int nSource = pContext->top();
	pContext->pop();
	int nTarget = pContext->top();
	pContext->pop();
	// 養成データコピー
	// コピー元
	Chara::CDataCharaTrain* pTrainSource = pContext->getApp()->getExec().getTrainData(nSource);
	// コピー先
	Chara::CDataCharaTrain* pTrain = pContext->getApp()->getExec().getTrainData(nTarget);
	// データコピー
	*pTrain = *pTrainSource;
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end