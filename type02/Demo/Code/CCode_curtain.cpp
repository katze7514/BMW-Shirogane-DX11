#include "stdafx.h"

#include "../CDemoScene.h"
#include "../CDemoMovieClip.h"
#include "CCode_curtain.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_curtain::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INIT:
	{
		pScene_->setCurtain(nSide_);
		pCurtain_ = pScene_->getCurtainMovie(nSide_);
		pCurtain_->OnReset(pContext);
		setState(WAIT);
	}
	break;

	case WAIT:
		if(pCurtain_->IsEnd()){ setState(END); pScene_->setCurtain(-1); }
	break;

	default: break;
	}
}

} // namespace Code end
} // namespace Demo end
} // namespace BMW end