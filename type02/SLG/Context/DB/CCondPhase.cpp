#include "stdafx.h"

#include "../CSLGContext.h"
#include "../CDataCharaSLG.h"

#include "CCondPhase.h"

namespace BMW{
namespace SLG{

bool CCondPhase::judg(CSLGContext* p)
{
#ifdef BMW_DEBUG
	CDbg().Out("PHASE %d %d %d %d %d",getCond(), getPhase(), getTurn(), IsTurn(p->getTurn()), IsPhase(p->getPhase()));
#endif
	return IsTurn(p->getTurn()) && IsPhase(p->getPhase());
}

} // namespace SLG end
} // namespace BMW end