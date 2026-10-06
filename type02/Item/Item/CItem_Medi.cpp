#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CItem_Medi.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CItem_Medi::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext)
{
	// HP全快だったら使えない
	return slg.getBattle().getBattleOffset().getHP()>0;
}

void CItem_Medi::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// HPを全快する
	slg.calcHP(-slg.getBattle().getMaxHP());
}

} // namespace Item end
} // namespace BMW end
