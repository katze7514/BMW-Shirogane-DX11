#include "stdafx.h"

#include "CMap_cursol.h"

namespace BMW{
namespace SLG{
namespace Map{

void CMap_cursol::OnInit(Task::CTaskContext* pContext)
{// スタックの上から
 // ステップ数、移動先Y,X と積まれてる
	int nStep = pContext->top();
	pContext->pop();
	int nY1 = pContext->top();
	pContext->pop();
	int nX1 = pContext->top();
	pContext->pop();
	
	// 現在のカーソル位置
	int nX,nY;
	pContext->getInput()->getCursolPos(nX,nY);
	
	motion_.setStart(nX,nY);
	motion_.setEnd(nX1,nY1);
	motion_.setStep(nStep);
	motion_.setCurrentStep(0);
	pContext->getInput()->guardCursol(true);
}

void CMap_cursol::OnAction(Task::CTaskContext* pContext)
{
	motion_.inc();
	pContext->getInput()->setCursolPos(motion_.getX(),motion_.getY());
	if(motion_.IsEnd())
	{// 移動が終わったら、リターン
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guardCursol(false);
	}
}

} // namespace Map end
} // namepsace SLG end
} // namespace BMW end