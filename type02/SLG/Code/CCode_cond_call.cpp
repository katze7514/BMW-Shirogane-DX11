#include "stdafx.h"

#include "../Context/DB/ISlgCond.h"
#include "../Context/CSLGContext.h"

#include "CCode_cond_call.h"

namespace BMW{
namespace SLG{
namespace Code{

CCode_cond_call::~CCode_cond_call()
{
	DELETE_SAFE(pCond_);
}

void CCode_cond_call::OnAction(Task::CTaskContext* pContext)
{
	// Condの結果がtrueだったら、設定されてる関数呼び出し
	if(pCond_->judg(static_cast<CSLGContext*>(pContext)))
		pContext->getTaskList()->getTaskListCtrl()->callTaskList(getState(),true);
	// どちらにしても、次のコードへ
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end