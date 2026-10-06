#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CItem_Giru.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
void CItem_Giru::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
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
