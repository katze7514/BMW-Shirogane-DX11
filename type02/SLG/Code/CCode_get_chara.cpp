/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CCode_get_chara.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_get_chara::OnAction(Task::CTaskContext* pContext)
{// ƒLƒƒƒ‰ƒXƒe‚ðŽæ“¾
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	CDataCharaSLG* pData = p->getTargetCharaData();
	
	switch(getState())
	{
	case LV:		p->push(pData->getBattle().getLv());	break;
	case PHASE:		p->push(pData->getPhase());				break;
	case HP:		p->push(pData->getBattle().getHP());	break;
	case HP_MAX:	p->push(pData->getBattle().getMaxHP()); break;
	case EN:		p->push(pData->getBattle().getEN());	break;
	case EN_MAX:	p->push(pData->getBattle().getMaxEN()); break;
	case SP:		p->push(pData->getBattle().getSP());	break;
	case SP_MAX:	p->push(pData->getBattle().getMaxSP()); break;
	case MENTAL:	p->push(pData->getBattle().getMental());break;
	case MAP:		p->push(pData->getIndex());				break;
	case ACT:		p->push(pData->getState().getAct());	break;
	case WAY:		p->push(pData->getState().getWay());	break;
	default:		p->push(-1); break;
	}
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
