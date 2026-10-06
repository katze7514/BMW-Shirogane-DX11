#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "../IDAbility.h"
#include "CAbility_Sprecover.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Sprecover::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().calcSP(-10);
}

void CAbility_Sprecover::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().calcSP(10);
}

} // namespace Ability end
} // namespace BMW end
