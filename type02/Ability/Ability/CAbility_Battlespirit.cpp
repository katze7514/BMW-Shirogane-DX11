#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Battlespirit.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Battlespirit::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().calcMental(5);
}

void CAbility_Battlespirit::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().calcMental(-5);
}

} // namespace Ability end
} // namespace BMW end
