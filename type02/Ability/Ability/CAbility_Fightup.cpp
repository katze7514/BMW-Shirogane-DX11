#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Fightup.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Fightup::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+2
	slg.getBattle().calcMental(2);
}

void CAbility_Fightup::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().calcMental(-2);
}

} // namespace Ability end
} // namespace BMW end
