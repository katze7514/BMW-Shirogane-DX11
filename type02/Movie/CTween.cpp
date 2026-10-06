#include "stdafx.h"

#include "CTween.h"

namespace BMW{
namespace Movie{

void CTween::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
		OnAction(pContext);
	else
		OnDraw(pContext);

	pTask_->Task(pContext);
}
void CTween::OnAction(Task::CTaskContext* pContext)
{
//	if(motion_.IsEnd()) OnReset(pContext);
	motion_.inc();
}

void CTween::OnDraw(Task::CTaskContext* pContext)
{
	//motion_.getCurrent().check();
	//CDbg().Out("%d %d %d",motion_.getWidth(),motion_.getHeight(),motion_.getAngle());
	setDrawInfo(motion_);
}

void CTween::OnReset(Task::CTaskContext* pContext)
{
	IKeyFrame::OnReset(pContext);
	motion_.reset();
	setDrawInfo(motion_.getStart());
}

void CTween::getDrawSize(LONG& lWidth, LONG& lHeight)const
{
	getSize(lWidth,lHeight);
	lWidth = roundRShift(lWidth * motion_.getWidth(), 8);
	lHeight = roundRShift(lHeight * motion_.getHeight(), 8);
}

} // namespace Movie end
} // namespace BMW end