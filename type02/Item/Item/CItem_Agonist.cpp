#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Chara/CDataCharaInter.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CItem_Agonist.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Agonist::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 移動力+1 到達+2 Tough+200 Quick+20
	battle.setMove(battle.getMaxMove()+1);
	battle.setJump(battle.getMaxJump()+2);
	battle.setTough(battle.getMaxTough()+200);
	battle.setQuick(battle.getMaxQuick()+20);
}

void CItem_Agonist::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	battle.setMove(battle.getMaxMove()-1);
	battle.setJump(battle.getMaxJump()-2);
	battle.setTough(battle.getMaxTough()-200);
	battle.setQuick(battle.getMaxQuick()-20);
}

void CItem_Agonist::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 移動力+1 到達+2 Tough+200 Quick+20
	inter.setItemMove(inter.getItemMove()+1);
	inter.setItemJump(inter.getItemJump()+2);
	inter.setItemTough(inter.getItemTough()+200);
	inter.setItemQuick(inter.getItemQuick()+20);
}

void CItem_Agonist::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemMove(inter.getItemMove()-1);
	inter.setItemJump(inter.getItemJump()-2);
	inter.setItemTough(inter.getItemTough()-200);
	inter.setItemQuick(inter.getItemQuick()-20);
}

void CItem_Agonist::applyWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// 射程と到達
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
	// HIT+20
 	weapon.setHit(weapon.getHit()+30);
}

void CItem_Agonist::backWeapon(Weapon::CDataWeaponBattle& weapon, int nAttr)
{
	// 射程と到達
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
	// HIT-20
 	weapon.setHit(weapon.getHit()-30);
}


} // namespace Item end
} // namespace BMW end
