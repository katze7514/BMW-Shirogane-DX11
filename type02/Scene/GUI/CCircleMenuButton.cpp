#include "stdafx.h"

#include "CCircleMenu.h"
#include "CCircleMenuButton.h"

namespace BMW{
namespace GUI{

void CCircleMenuButton::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			OnAction(pContext);
			pTask_->Task(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			pTask_->Task(pContext);
		}
	}
}

void CCircleMenuButton::OnReset(Task::CTaskContext* pContext)
{
	setState(MOTION);
	// 退場時なら、ENDからSTARTへ
	if(getParent()->getState()==CCircleMenu::EXIT)
		motion_.swap();
	
	motion_.reset();
	//motion_.setCurrentStep(motion_.getStep());
	pTask_->OnReset(pContext);
	pTask_->valid(false);
}

void CCircleMenuButton::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==MOTION)
	{
		if(motion_.inc())
		{// 動作終了
			setState(NORMAL);
			if(getParent()->getState()==CCircleMenu::INTRO)
				pTask_->valid(true);

			// 動作終了を管理クラスに知らせる
			pContext->getTaskList()->removeMe();
		}
	}
}

const Draw::CDrawInfo CCircleMenuButton::getDrawInfo(bool bRela)
{
	if(bRela) // 親がいないということ自体が間違い
		return getParent()->getDrawInfo().calcAbsolute(motion_);
	else
		return motion_.getCurrent();
}

void CCircleMenuButton::getDrawSize(LONG& lWidth, LONG& lHeight)const
{
	getSize(lWidth,lHeight);
	lWidth = roundRShift(lWidth * motion_.getWidth(), 8);
	lHeight = roundRShift(lHeight * motion_.getHeight(), 8);
}


} // namespace GUI end
} // namespace BMW end