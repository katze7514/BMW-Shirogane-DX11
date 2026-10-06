#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Event/CEvent.h"

#include "CCode_del_symbol.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_del_symbol::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nNo = p->top();
	p->pop();
	
	// エフェクトリストから削除
	p->getEvent()->delEffect(nNo);
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end