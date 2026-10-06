#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Futureeye_Second.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// g—p
/////////////////////////////////////////////
bool CAbility_Futureeye_Second::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	int nRand = CApp::rand_.Get(100)+1;
#ifdef BMW_DEBUG
	CDbg().Out("SECNOD %d",nRand);
#endif
	// ‹C—Í110ˆÈã‚Å50%
	return slg.getBattle().getMental()>=110 && nRand<=50;
}

} // namespace Ability end
} // namespace BMW end
