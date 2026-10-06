#include "stdafx.h"

#include "../../SLG/Context/COffsetWeapon.h"

#include "CAbility_Magician.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// •â³
////////////////////////////////////////////
void CAbility_Magician::applyOffset(SLG::COffsetRange& data, int nAttr)
{
	const CMagicianTableBase& mgc = magicianTable_.getTable(nAttr-1);
	// Ë’ö
	data.calcMax(mgc.getRange());
}

void CAbility_Magician::applyOffset(SLG::COffsetHit& data, int nAttr)
{
	const CMagicianTableBase& mgc = magicianTable_.getTable(nAttr-1);
	// –½’†
	data.calcHit(mgc.getHit());
	// ‰ñ”ğ
	data.calcAvoid(mgc.getAvoid());
}

} // namespace Ability end
} // namespace BMW end
