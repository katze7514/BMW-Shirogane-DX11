#include "stdafx.h"

#include "../CADVContext.h"
#include "../CADVBack.h"

#include "CBack_change.h"

namespace BMW{
namespace ADV{
namespace API{

void CBack_change::OnAction(Task::CTaskContext* pContext)
{
	// スタックトップに変更する背景ID
	int nBack = pContext->top();
	pContext->pop();

	CADVContext* p = static_cast<CADVContext*>(pContext);
	CBackDB& db = p->getBackDB();
	smart_ptr<CADVBack>& pBack = p->getBack();

	db.setBack(pBack->getBack(), pBack->getBackName(), nBack);
	pBack->getBackName()->UpdateText();

	// 終了したら、リターン
	getTaskListCtrl()->returnTaskList();
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end