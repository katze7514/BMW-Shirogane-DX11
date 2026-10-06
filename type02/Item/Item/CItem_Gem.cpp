#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Gem.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Gem::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 最大EN+100
	battle.setEN(battle.getMaxEN()+100);
}

void CItem_Gem::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 最大EN-100
	battle.setEN(battle.getMaxEN()-100);
}

void CItem_Gem::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 最大EN+100
	inter.setItemEN(inter.getItemEN()+100);
}

void CItem_Gem::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 最大EN-100
	inter.setItemEN(inter.getItemEN()-100);
}

} // namespace Item end
} // namespace BMW end
