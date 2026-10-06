#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "CItem_Archer.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Archer::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{// 射程UP
 // アイテムはSLGなどで動的に変化しないので、パラメタに加算してしまう
	if(weapon.getCoreMin()!=weapon.getCoreMax()
	&& weapon.getKind()!=Weapon::Kind::CURE
	&& weapon.getKind()!=Weapon::Kind::REFILL
	&& weapon.getKind()!=Weapon::Kind::STATUS
	&& !weapon.IsF())
	{
		weapon.setMax(weapon.getMax()+1);
		weapon.setCoreMax(weapon.getCoreMax()+1);
		weapon.setHeight(weapon.getHeight()+4);
	}
	weapon.setHit(weapon.getHit()+20);
}

void CItem_Archer::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	if(weapon.getCoreMin()!=weapon.getCoreMax()
	&& weapon.getKind()!=Weapon::Kind::CURE
	&& weapon.getKind()!=Weapon::Kind::REFILL
	&& weapon.getKind()!=Weapon::Kind::STATUS
	&& !weapon.IsF())
	{
		weapon.setMax(weapon.getMax()-1);
		weapon.setCoreMax(weapon.getCoreMax()-1);
		weapon.setHeight(weapon.getHeight()-4);
	}
	weapon.setHit(weapon.getHit()-20);
}

} // namespace Item end
} // namespace BMW end
