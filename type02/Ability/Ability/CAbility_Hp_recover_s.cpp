#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Hp_recover_s.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Hp_recover_s::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.calcHP(-slg.getBattle().getMaxHP()/10);
}

void CAbility_Hp_recover_s::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.calcHP(slg.getBattle().getMaxHP()/10);
}

} // namespace Ability end
} // namespace BMW end
