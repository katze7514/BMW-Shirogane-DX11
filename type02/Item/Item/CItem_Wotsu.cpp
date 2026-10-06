#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Wotsu.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Wotsu::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP +1000、Tough +150
	battle.setHP(battle.getMaxHP()+1000);
	battle.setTough(battle.getMaxTough()+150);
}

void CItem_Wotsu::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP -1000、Tough -150
	battle.setHP(battle.getMaxHP()-1000);
	battle.setTough(battle.getMaxTough()-150);
}

void CItem_Wotsu::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP +1000、Tough +150
	inter.setItemHP(inter.getItemHP()+1000);
	inter.setItemTough(inter.getItemTough()+150);
}

void CItem_Wotsu::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP -1000、Tough -150
	inter.setItemHP(inter.getItemHP()-1000);
	inter.setItemTough(inter.getItemTough()-150);
}

} // namespace Item end
} // namespace BMW end
