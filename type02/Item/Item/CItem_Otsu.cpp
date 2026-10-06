#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Otsu.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Otsu::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP +500、Tough +100
	battle.setHP(battle.getMaxHP()+500);
	battle.setTough(battle.getMaxTough()+100);
}

void CItem_Otsu::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP -500、Tough -100
	battle.setHP(battle.getMaxHP()-500);
	battle.setTough(battle.getMaxTough()-100);
}

void CItem_Otsu::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP +500、Tough +100
	inter.setItemHP(inter.getItemHP()+500);
	inter.setItemTough(inter.getItemTough()+100);
}

void CItem_Otsu::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP -500、Tough -100
	inter.setItemHP(inter.getItemHP()-500);
	inter.setItemTough(inter.getItemTough()-100);
}

} // namespace Item end
} // namespace BMW end
