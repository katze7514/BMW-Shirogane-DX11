#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Death_True.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
void CAbility_Death_True::applyOffset(SLG::COffsetHit& data)
{
	data.calcHitOff(15);
	data.calcAvoid(15);
}

void CAbility_Death_True::applyOffset(SLG::COffsetBattle& data)
{
	data.calcCT(20);
}

bool CAbility_Death_True::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{ 
	// 気力110以上で発動
	return slg.getBattle().getMental()>=110;
}

bool CAbility_Death_True::enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p)
{
	// EN足りるよね？
	if(base.getBattle().getEN()>=getEN()+nAttr
	&& base.getBattle().getMental()>=130)
	{// 気力130以上、かつ相手との技量差による確率で発動
		return (base.getBattle().getSkill() >= target.getBattle().getSkill()
				? 75 : 10)>=(int)CApp::rand_.Get(100)+1;
	}

	return false;
}

} // namespace Ability end
} // namespace BMW end
