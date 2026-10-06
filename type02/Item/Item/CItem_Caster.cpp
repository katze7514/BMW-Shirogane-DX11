#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "CItem_Caster.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Caster::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN10%回復
	slg.getBattle().calcEN(-slg.getBattle().getMaxEN()/10);
}

void CItem_Caster::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// EN10%回復
	slg.getBattle().calcEN(slg.getBattle().getMaxEN()/10);
}

} // namespace Item end
} // namespace BMW end
