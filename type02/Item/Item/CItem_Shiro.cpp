#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CItem_Shiro.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CItem_Shiro::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	return slg.getBattle().getFundOffset().getSP()>0;
}

void CItem_Shiro::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	slg.getBattle().calcSP(-50);
}

} // namespace Item end
} // namespace BMW end
