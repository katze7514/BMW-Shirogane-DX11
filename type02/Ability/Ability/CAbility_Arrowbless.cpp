#include "stdafx.h"

#include "../../SLG/Context/COffsetWeapon.h"
#include "CAbility_Arrowbless.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ï‚ê≥íl
////////////////////////////////////////////
void CAbility_Arrowbless::applyOffset(SLG::COffsetHit& data)
{
	// âÒî+10
	data.calcAvoid(+10);
}

} // namespace Ability end
} // namespace BMW end
