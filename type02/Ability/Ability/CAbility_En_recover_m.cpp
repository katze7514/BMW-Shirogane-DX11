#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_En_recover_m.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_En_recover_m::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN20%回復
	slg.getBattle().calcEN(-(slg.getBattle().getMaxEN()*2)/10);
}

void CAbility_En_recover_m::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN20%回復
	slg.getBattle().calcEN((slg.getBattle().getMaxEN()*2)/10);
}

} // namespace Ability end
} // namespace BMW end
