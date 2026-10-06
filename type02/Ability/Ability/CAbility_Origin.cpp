#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Origin.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Origin::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 防御+30
	slg.getBattle().calcDefence(30);
}

void CAbility_Origin::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 防御-30
	slg.getBattle().calcDefence(-30);
}
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
void CAbility_Origin::applyOffset(SLG::COffsetHit& data)
{
	// 命中+30
	data.calcHit(30);
}

void CAbility_Origin::applyOffset(SLG::COffsetBattle& data)
{
	// CT+15
	data.calcCT(15);
}

} // namespace Ability end
} // namespace BMW end
