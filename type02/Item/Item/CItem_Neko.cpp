#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Neko.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Neko::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick+5
	battle.setQuick(battle.getMaxQuick()+5);
}

void CItem_Neko::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// Quick-5
	battle.setQuick(battle.getMaxQuick()-5);
}

void CItem_Neko::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemQuick(inter.getItemQuick()+5);
}

void CItem_Neko::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemQuick(inter.getItemQuick()-5);
}

} // namespace Item end
} // namespace BMW end
