#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "CItem_Curry.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CItem_Curry::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	return slg.getBattle().getBattleOffset().getEN()>0;
}

void CItem_Curry::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN‘S‰ñ•œ
	slg.getBattle().calcEN(-slg.getBattle().getMaxEN());
}

} // namespace Item end
} // namespace BMW end
