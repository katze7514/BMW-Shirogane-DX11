#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "CItem_True_assassin.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_True_assassin::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// HIT+30、CT+20
 	weapon.setHit(weapon.getHit()+30);
	weapon.setCT(weapon.getCT()+20);
}

void CItem_True_assassin::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// HIT-30、CT-20
 	weapon.setHit(weapon.getHit()-30);
	weapon.setCT(weapon.getCT()-20);
}

} // namespace Item end
} // namespace BMW end
