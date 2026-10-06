#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Hp_recover_m.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Hp_recover_m::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP20%回復
	slg.calcHP(-(slg.getBattle().getMaxHP()*2)/10);
}

void CAbility_Hp_recover_m::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP20%回復
	slg.calcHP((slg.getBattle().getMaxHP()*2)/10);
}

} // namespace Ability end
} // namespace BMW end
