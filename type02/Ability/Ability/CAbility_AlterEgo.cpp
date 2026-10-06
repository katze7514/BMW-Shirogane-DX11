#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_AlterEgo.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CAbility_AlterEgo::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN‘«‚è‚Ä‹C—Í130ˆÈã‚Å2•ª‚Ì1
	return slg.getBattle().getEN()>=getEN()+nAttr
		&& slg.getBattle().getMental()>=130 
		&& CApp::rand_.Get(2)==0;
}

} // namespace Ability end
} // namespace BMW end
