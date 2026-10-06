#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CCharaState.h"

#include "CMapChipChara2.h"

namespace BMW{
namespace SLG{
namespace Map{

//bool CMapChipChara2::IsMove(){ return bMove_; }
//void CMapChipChara2::move(bool bMove){ bMove_=bMove; }
//bool CMapChipChara2::bMove_=false;

/*void CMapChipChara2::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid()){
			OnAction(pContext);
			callTaskAction(pContext);
		}
	}
	else
	{
		if(IsVisible()){
			if(!IsMove() || pContext->getValue(Flag::CTRL_CHARA)==getID())
				drawInfo_.setAlpha(255);
			else
				drawInfo_.setAlpha(127);

			callTaskDraw(pContext);
		}
	}
}*/

void CMapChipChara2::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{// Œ»Ý‚ÌŒü‚«‚Æó‘Ô‚É‚ ‚í‚¹‚Ä‘I‘ð
	case STAND:
		getTask(CHARA)->setState(getStand()+pState_->getWay());
	break;

	default:
		getTask(CHARA)->setState(getState()+pState_->getWay());
	break;
	}
}

int CMapChipChara2::getStand()
{
	switch(pState_->getAct())
	{
	case Act::BEFORE:
	case Act::MOVE:
	case Act::HIT_AWAY:
	case Act::HIT_AWAY_MOVE:
	case Act::DEATH_EVENT:
	case Act::REMOVE:
		return pState_->IsPinch() ? ChipMovie::BEFORE_PINCH_TOP : ChipMovie::BEFORE_TOP;

	default: 
		return pState_->IsPinch() ? ChipMovie::AFTER_PINCH_TOP : ChipMovie::AFTER_TOP;
	}
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end