#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Ikiyoyo.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Ikiyoyo::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+1
	slg.getBattle().calcMental(1);
}

void CAbility_Ikiyoyo::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力-1
	slg.getBattle().calcMental(-1);
}

} // namespace Ability end
} // namespace BMW end
