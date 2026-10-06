#include "stdafx.h"

#include "../../Victory/CVictory_check.h"
#include "../CSLGContext.h"
#include "CCondDeath.h"

namespace BMW{
namespace SLG{

bool CCondDeath::judg(CSLGContext* p)
{
#ifdef BMW_DEBUG
	CDbg().Out("DEATH %d %d %d",getType(),getPhase(),Victory::CVictory_check::IsDeathPhaseOne(getPhase(),*p));
#endif

	if(getType()==ALL)	return Victory::CVictory_check::IsDeathPhase(getPhase(),*p);
	ef(getType()==ONE)	return Victory::CVictory_check::IsDeathPhaseOne(getPhase(),*p);
	ef(getType()==ALL_ALIVE)	return Victory::CVictory_check::IsAlivePhase(getPhase(),*p);
	ef(getType()==ONE_ALIVE)	return Victory::CVictory_check::IsAlivePhaseOne(getPhase(),*p);
	return false;
}

bool CCondDeathLive::judg(CSLGContext* p)
{
	return Victory::CVictory_check::IsDeathPhase(getPhase(),*p,getSetChara());
}

} // namespace SLG end
} // namespace BMW end