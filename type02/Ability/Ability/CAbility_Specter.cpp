#include "stdafx.h"

#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Specter.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Specter::applyOffset(SLG::COffsetHit& data, int nAttr)
{
	const CSpecterTableBase& spc = specterTable_.getTable(nAttr-1);
	data.calcHit(spc.getHit());
	data.calcAvoid(spc.getAvoid());
}

void CAbility_Specter::applyOffset(SLG::COffsetBattle& data, int nAttr)
{
	const CSpecterTableBase& spc = specterTable_.getTable(nAttr-1);
	data.calcAttack(spc.getAttack());
}

} // namespace Ability end
} // namespace BMW end
