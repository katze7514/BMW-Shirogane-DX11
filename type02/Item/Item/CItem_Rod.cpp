#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "CItem_Rod.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Rod::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// CT+20
	weapon.setCT(weapon.getCT()+20);
}

void CItem_Rod::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// CT-20
	weapon.setCT(weapon.getCT()-20);
}

} // namespace Item end
} // namespace BMW end
