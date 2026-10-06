#include "stdafx.h"

#include "../mode.h" 

#include "../SLG/IDSLG.h"

#include "CDataScenario.h"

namespace BMW{
namespace Scenario{

int CDataScenario::getExpertRank(int nExpert)
{
#ifdef EXPERT_NORMAL
	// NORMALƒ‚[ƒh
	return SLG::Expert::NORMAL;
#else
	// getNormal()–¢–ž‚¾‚Á‚½‚çNORMAL
	if(getNormal()>nExpert) return SLG::Expert::NORMAL;
	else					return SLG::Expert::HARD;
#endif
}

} // namespace Scenario end
} // naemespace BMW end