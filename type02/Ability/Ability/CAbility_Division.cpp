#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"

#include "CAbility_Division.h"

namespace BMW{
namespace Ability{
/////////////////////////////////////////////
// ステータス補正値
/////////////////////////////////////////////
void CAbility_Division::applyOffset(SLG::COffsetHit& data, const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target)
{
	// 技量差に応じて、命中・回避UP
	int nHit;
	if(base.getBattle().getSkill()>=target.getBattle().getSkill())
		nHit=30;
	else
		nHit=10;
	data.calcHit(nHit);
	data.calcAvoid(nHit);
}

} // namespace Ability end
} // namespace BMW end
