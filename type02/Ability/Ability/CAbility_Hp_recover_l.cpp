#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Hp_recover_l.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Hp_recover_l::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP30%回復
	slg.calcHP(-(slg.getBattle().getMaxHP()*3)/10);
}

void CAbility_Hp_recover_l::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP30%回復
	slg.calcHP((slg.getBattle().getMaxHP()*3)/10);
}

} // namespace Ability end
} // namespace BMW end
