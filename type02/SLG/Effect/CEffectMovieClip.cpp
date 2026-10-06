#include "stdafx.h"

#include "CEffectMovieClip.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CEffectMovieClip::OnAction(Task::CTaskContext* pContext)
{
	// 終了したら、非表示になり、リストからはずれる
	if(IsEnd())
	{ 
		visible(false);
		if(IsRemove()) pContext->getTaskList()->removeMe();
	}
}

void CEffectMovieClip::OnReset(Task::CTaskContext* pContext)
{
	CMovieClip::OnReset(pContext);
	visible(true);
}

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end
