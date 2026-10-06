#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../Chara/CDataCharaInter.h"

#include "CItem_Ribbon.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Ribbon::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 気力+5
	battle.calcMental(5);
}

void CItem_Ribbon::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	Chara::CDataCharaBattle& battle = slg.getBattle();
	// 気力-5
	battle.calcMental(-5);
}

void CItem_Ribbon::applyStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 気力+5
	inter.calcItemMental(5);
}

void CItem_Ribbon::backStatus(Chara::CDataCharaInter& inter, int nAttr)
{
	// 気力-5
	inter.calcItemMental(-5);
}

} // namespace Item end
} // namespace BMW end
