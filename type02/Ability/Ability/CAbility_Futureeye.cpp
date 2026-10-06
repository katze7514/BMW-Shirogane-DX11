#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Futureeye.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CAbility_Futureeye::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN‚ª‘«‚è‚Ä‹C—Í130ˆÈã‚Å3•ª‚Ì1
	return slg.getBattle().getEN()>=getEN()+nAttr
		&& slg.getBattle().getMental()>=130
		&& CApp::rand_.Get(3)==0
		;
}

} // namespace Ability end
} // namespace BMW end
