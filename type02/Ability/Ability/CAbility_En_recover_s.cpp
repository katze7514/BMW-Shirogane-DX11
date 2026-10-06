#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_En_recover_s.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_En_recover_s::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN10%回復
	slg.getBattle().calcEN(-slg.getBattle().getMaxEN()/10);
}

void CAbility_En_recover_s::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN10%回復
	slg.getBattle().calcEN(slg.getBattle().getMaxEN()/10);
}

} // namespace Ability end
} // namespace BMW end
