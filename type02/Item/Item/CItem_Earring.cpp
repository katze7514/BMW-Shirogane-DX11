#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Earring.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Earring::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 気力+10
	battle.calcMental(10);
}

void CItem_Earring::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 気力-10
	battle.calcMental(-10);
}

void CItem_Earring::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 気力+10
	inter.calcItemMental(10);
}

void CItem_Earring::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 気力-10
	inter.calcItemMental(-10);
}

} // namespace Item end
} // namespace BMW end
