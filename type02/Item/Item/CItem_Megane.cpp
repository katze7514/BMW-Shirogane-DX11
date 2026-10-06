#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "CItem_Megane.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Megane::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// 命中+10
	weapon.setHit(weapon.getHit()+10);
}

void CItem_Megane::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// 命中-10
	weapon.setHit(weapon.getHit()-10);
}

} // namespace Item end
} // namespace BMW end
