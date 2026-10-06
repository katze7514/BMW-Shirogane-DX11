#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"

#include "CAbility_Breakline.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// 補正
////////////////////////////////////////////
void CAbility_Breakline::applyOffset(SLG::COffsetHit& data)
{
	// 命中・回避+10
	data.calcHitOff(10);
	data.calcAvoid(10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Breakline::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上
	return slg.getBattle().getMental()>=130;
}

} // namespace Ability end
} // namespace BMW end
