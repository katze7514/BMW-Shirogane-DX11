#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "CItem_Kyudo.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Kyudo::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	if(weapon.getCoreMin()!=weapon.getCoreMax()
	&& weapon.getKind()!=Weapon::Kind::CURE
	&& weapon.getKind()!=Weapon::Kind::REFILL
	&& weapon.getKind()!=Weapon::Kind::STATUS
	&& !weapon.IsF())
	{
		weapon.setMax(weapon.getMax()+1);
		weapon.setCoreMax(weapon.getCoreMax()+1);
		weapon.setHeight(weapon.getHeight()+2);
	}
}

void CItem_Kyudo::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	if(weapon.getCoreMin()!=weapon.getCoreMax()
	&& weapon.getKind()!=Weapon::Kind::CURE
	&& weapon.getKind()!=Weapon::Kind::REFILL
	&& weapon.getKind()!=Weapon::Kind::STATUS
	&& !weapon.IsF())
	{
		weapon.setMax(weapon.getMax()-1);
		weapon.setCoreMax(weapon.getCoreMax()-1);
		weapon.setHeight(weapon.getHeight()-2);
	}
}

} // namespace Item end
} // namespace BMW end
