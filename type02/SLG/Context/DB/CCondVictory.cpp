#include "stdafx.h"

#include "../CSLGContext.h"
#include "../CSLGDef.h"
#include "CCondVictory.h"

namespace BMW{
namespace SLG{

bool CCondVictory::judg(CSLGContext* p)
{
	switch(getType())
	{
	// ”s–kðŒ—Dæ
	case VICTORY:	 return !p->getSLGDef().IsLose(p) && p->getSLGDef().IsVictory(p);
	case LOSE:		 return p->getSLGDef().IsLose(p);
	case EXPERT:	 return p->getSLGDef().IsExpert(p);
	default:		 return false;
	}
}

} // namespace SLG end
} // namespace BMW end