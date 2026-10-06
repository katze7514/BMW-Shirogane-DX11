#include "stdafx.h"

#include "../SLG/IDSLG.h"
#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "CDataWeaponBattleRefill.h"

namespace BMW{
namespace Weapon{

bool CDataWeaponBattleRefill::enableTargetOne(const SLG::CDataCharaSLG& slg, SLG::CSLGContext& pContext)
{
	// 全快の相手には効果無し
	// ENチェック
	if(slg.getBattle().getBattleOffset().getEN()>0) return true;
	// 武器弾数チェック
	const Chara::CDataCharaBattle& battle = slg.getBattle();
	Weapon::CDataWeaponBattle* pWeapon;
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = pContext.getWeaponData(*battle.nextWeapon());
		if(pWeapon==NULL) continue;
		// 弾数だけチェック
		if(pWeapon->getBallet()!=pWeapon->getBalletRest()) return true;
	}
	return false;
}

} // namespace Weapon end
} // namespace BMW end