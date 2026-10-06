#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "../IDItem.h"
#include "CItem_Magazine.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CItem_Magazine::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	const Chara::CDataCharaBattle& battle = slg.getBattle();
	Weapon::CDataWeaponBattle* pWeapon;
	// 武器弾数全開？
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = p.getWeaponData(*battle.nextWeapon());
		if(pWeapon->getBallet()!=pWeapon->getBalletRest()) return true;
	}
	return false;
}

void CItem_Magazine::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	if(slg.IsWeaponLoad())
	{// ロードされてないとね……
		Chara::CDataCharaBattle& battle = slg.getBattle();
		// 武器弾数全開
		battle.beginWeapon();
		while(!battle.endWeapon())
			p.getWeaponData(*battle.nextWeapon())->refill();
	}
}

} // namespace Item end
} // namespace BMW end
