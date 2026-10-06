#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Death_Ex.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
void CAbility_Death_Ex::applyOffset(SLG::COffsetHit& data)
{
	data.calcHitOff(10);
	data.calcAvoid(10);
}

void CAbility_Death_Ex::applyOffset(SLG::COffsetBattle& data)
{
	data.calcCT(20);
}

bool CAbility_Death_Ex::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{ 
	// 気力110以上で発動
	return slg.getBattle().getMental()>=110;
}

bool CAbility_Death_Ex::enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p)
{
	// 気力140以上で発動
	return base.getBattle().getMental()>=140;
}

} // namespace Ability end
} // namespace BMW end
