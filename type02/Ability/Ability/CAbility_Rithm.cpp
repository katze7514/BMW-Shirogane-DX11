#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "../IDAbility.h"
#include "CAbility_Rithm.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Rithm::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+1
	slg.getBattle().calcMental(1);
}

void CAbility_Rithm::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力-1
	slg.getBattle().calcMental(-1);
}

} // namespace Ability end
} // namespace BMW end
