#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "../IDItem.h"
#include "CItem_Berserker.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Berserker::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP +1500、Tough +250
	battle.setHP(battle.getMaxHP()+1500);
	battle.setTough(battle.getMaxTough()+250);
}

void CItem_Berserker::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// HP -1500、Tough -250
	battle.setHP(battle.getMaxHP()-1500);
	battle.setTough(battle.getMaxTough()-250);
}

void CItem_Berserker::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP +1500、Tough +250
	inter.setItemHP(inter.getItemHP()+1500);
	inter.setItemTough(inter.getItemTough()+250);
}

void CItem_Berserker::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// HP -1500、Tough -250
	inter.setItemHP(inter.getItemHP()-1500);
	inter.setItemTough(inter.getItemTough()-250);
}

} // namespace Item end
} // namespace BMW end
