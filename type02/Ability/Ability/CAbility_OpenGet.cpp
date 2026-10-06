#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_OpenGet.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CAbility_OpenGet::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN‘«‚è‚Ä‹C—Í120ˆÈã‚Å4•ª‚Ì1
	return slg.getBattle().getEN()>=getEN()+nAttr
		&& slg.getBattle().getMental()>=120 
		&& CApp::rand_.Get(4)==0;
}

} // namespace Ability end
} // namespace BMW end
