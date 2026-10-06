#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "../IDItem.h"
#include "CItem_Rider.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Rider::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 移動力+2 到達+4
	battle.setMove(battle.getMaxMove()+2);
	battle.setJump(battle.getMaxJump()+4);
}

void CItem_Rider::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 移動力-2 到達-4
	battle.setMove(battle.getMaxMove()-2);
	battle.setJump(battle.getMaxJump()-4);
}

void CItem_Rider::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 移動力+2 到達+4
	inter.setItemMove(inter.getItemMove()+2);
	inter.setItemJump(inter.getItemJump()+4);
}

void CItem_Rider::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 移動力-2 到達-4
	inter.setItemMove(inter.getItemMove()-2);
	inter.setItemJump(inter.getItemJump()-4);
}

} // namespace Item end
} // namespace BMW end
