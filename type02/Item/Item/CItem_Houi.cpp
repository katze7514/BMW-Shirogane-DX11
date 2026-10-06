#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Houi.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Houi::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick+15
	battle.setQuick(battle.getMaxQuick()+15);
}

void CItem_Houi::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick-15
	battle.setQuick(battle.getMaxQuick()-15);
}

void CItem_Houi::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemQuick(inter.getItemQuick()+15);
}

void CItem_Houi::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemQuick(inter.getItemQuick()-15);
}

} // namespace Item end
} // namespace BMW end
