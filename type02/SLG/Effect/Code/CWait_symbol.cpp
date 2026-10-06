#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Event/CEvent.h"

#include "../CEffectInfo.h"
#include "CWait_symbol.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CWait_symbol::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nNo = p->top();
	p->pop();

	pMovie_ = static_cast<Movie::CMovieClip*>(p->getEvent()->getEffect(nNo)->pEffect_);
}

void CWait_symbol::OnAction(Task::CTaskContext* pContext)
{
	// ムービーが終了してたらリターン
	if(pMovie_->IsEnd()) getTaskListCtrl()->returnTaskList();
}

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end