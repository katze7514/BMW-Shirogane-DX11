#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Maid.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Maid::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// SPUP+20
	battle.setSP(battle.getMaxSP()+20);
}

void CItem_Maid::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// SPUP-20
	battle.setSP(battle.getMaxSP()-20);
}

void CItem_Maid::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// SPUP+20
	inter.calcItemSP(20);
}

void CItem_Maid::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// SPUP-20
	inter.calcItemSP(-20);
}

} // namespace Item end
} // namespace BMW end
