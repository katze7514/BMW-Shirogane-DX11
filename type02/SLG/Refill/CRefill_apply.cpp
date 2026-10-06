#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "CRefill_apply.h"

namespace BMW{
namespace SLG{
namespace Refill{

void CRefill_apply::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->getTargetCharaData()->refill(p);

	// Œø‰Ê”­“®‚µ‚½‚Ì‚ÅƒŠƒ^[ƒ“
	getTaskListCtrl()->returnTaskList();
}

} // namespace Refill end
} // namespace SLG end
} // namespace BMW end