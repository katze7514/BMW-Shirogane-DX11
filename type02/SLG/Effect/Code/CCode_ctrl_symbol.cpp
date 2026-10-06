#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Event/CEvent.h"
#include "../CEffectInfo.h"

#include "CCode_ctrl_symbol.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_ctrl_symbol::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nNo = p->top();
	p->pop();
	int nType = p->top();
	p->pop();
	int nValue = p->top();
	p->pop();
	
	// エフェクト取得
	CEffectInfo *pInfo = p->getEvent()->getEffect(nNo);
	// 操作
	switch(nType)
	{
	case VALID:		pInfo->pEffect_->valid(nValue);		break;
	case VISIBLE:	pInfo->pEffect_->visible(nValue);	break;
	default: break;
	}
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end