#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Superpower.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Superpower::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 腕力+30
	slg.getBattle().calcStrength(30);
}

void CAbility_Superpower::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 腕力-30
	slg.getBattle().calcStrength(-30);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Superpower::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力120以上
	return slg.getBattle().getMental()>=120;
}

} // namespace Ability end
} // namespace BMW end
