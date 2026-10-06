#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Lancer.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Lancer::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick+20、Move+1
	battle.setQuick(battle.getMaxQuick()+20);
	battle.setMove(battle.getMaxMove()+1);
}

void CItem_Lancer::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick-20、Move-1
	battle.setQuick(battle.getMaxQuick()-20);
	battle.setMove(battle.getMaxMove()-1);
}

void CItem_Lancer::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// Quick+20、Move+1
	inter.setItemQuick(inter.getItemQuick()+20);
	inter.setItemMove(inter.getItemMove()+1);
}

void CItem_Lancer::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// Quick-20、Move-1
	inter.setItemQuick(inter.getItemQuick()-20);
	inter.setItemMove(inter.getItemMove()-1);
}

} // namespace Item end
} // namespace BMW end
