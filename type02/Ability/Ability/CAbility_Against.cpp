#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Against.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Against::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+2
	slg.getBattle().calcMental(2);
}

void CAbility_Against::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力-2
	slg.getBattle().calcMental(-2);
}

} // namespace Ability end
} // namespace BMW end
