#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Fly.h"

namespace BMW{
namespace Ability{
/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CAbility_Fly::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// ‹C—Í130ˆÈã
	return slg.getBattle().getMental()>=130;
}

} // namespace Ability end
} // namespace BMW end
