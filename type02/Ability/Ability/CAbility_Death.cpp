#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Death.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
void CAbility_Death::applyOffset(SLG::COffsetHit& data)
{
	data.calcAvoid(10);
	data.calcHit(10);
}

void CAbility_Death::applyOffset(SLG::COffsetBattle& data)
{
	data.calcCT(10);
}

bool CAbility_Death::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{ 
	// ‹C—Í120ˆÈã‚Å”­“®
	return slg.getBattle().getMental()>=120;
}

} // namespace Ability end
} // namespace BMW end
