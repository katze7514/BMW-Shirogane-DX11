#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Boot.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Boot::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 移動力+1 到達+2
	battle.setMove(battle.getMaxMove()+1);
	battle.setJump(battle.getMaxJump()+2);
}

void CItem_Boot::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 移動力+1 到達+2
	battle.setMove(battle.getMaxMove()-1);
	battle.setJump(battle.getMaxJump()-2);
}

void CItem_Boot::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 移動力+1 到達+2
	inter.setItemMove(inter.getItemMove()+1);
	inter.setItemJump(inter.getItemJump()+2);
}

void CItem_Boot::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	inter.setItemMove(inter.getItemMove()-1);
	inter.setItemJump(inter.getItemJump()-2);
}

} // namespace Item end
} // namespace BMW end
