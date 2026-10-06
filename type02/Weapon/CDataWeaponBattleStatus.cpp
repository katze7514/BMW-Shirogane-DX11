#include "stdafx.h"

#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "CDataWeaponBattleStatus.h"

namespace BMW{
namespace Weapon{

void CDataWeaponBattleStatus::apply(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context)
{	// ステータスを増やす
	Chara::CDataCharaBattle& battle = chara.getBattle();

	battle.calcStrength(getStrengthAid());
	battle.calcMagic(getMagicAid());
	battle.calcAvoid(getAvoidAid());
	battle.calcHit(getHitAid());
	battle.calcDefence(getDefenceAid());
	battle.calcSkill(getSkillAid());
	battle.calcMental(getMentalAid());
}

bool CDataWeaponBattleStatus::enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context)
{
	// 味方にできる
	return target.IsLive() && chara.getPhase()==target.getPhase();
}

} // namespace Weapon end
} // namespace BMW end