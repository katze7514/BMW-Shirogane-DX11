#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CItem_Saber.h"

namespace BMW{
namespace Item{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CItem_Saber::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.calcHP(-slg.getBattle().getMaxHP()/10);
}

void CItem_Saber::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.calcHP(slg.getBattle().getMaxHP()/10);
}

} // namespace Item end
} // namespace BMW end
