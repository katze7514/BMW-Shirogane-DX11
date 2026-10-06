#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_En_recover_l.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_En_recover_l::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN30%回復
	slg.getBattle().calcEN(-(slg.getBattle().getMaxEN()*3)/10);
}

void CAbility_En_recover_l::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN30%回復
	slg.getBattle().calcEN((slg.getBattle().getMaxEN()*3)/10);
}

} // namespace Ability end
} // namespace BMW end
