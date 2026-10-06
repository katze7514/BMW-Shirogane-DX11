#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "../IDItem.h"
#include "CItem_Mabo.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CItem_Mabo::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	const Chara::CDataCharaBattle& battle = slg.getBattle();
	Weapon::CDataWeaponBattle* pWeapon;
	// 武器弾数全開？
	bool bWeapon = false;
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = p.getWeaponData(*battle.nextWeapon());
		bWeapon = pWeapon->getBallet()!=pWeapon->getBalletRest() || bWeapon;
	}

	return battle.getBattleOffset().getHP()>0 || battle.getBattleOffset().getEN()>0 || bWeapon;
}

void CItem_Mabo::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 武器弾数全開
	battle.beginWeapon();
	while(!battle.endWeapon())
		p.getWeaponData(*battle.nextWeapon())->refill();

	// HP全快
	slg.calcHP(-battle.getMaxHP());
	// EN全快
	battle.calcEN(-battle.getMaxEN());
}

} // namespace Item end
} // namespace BMW end
