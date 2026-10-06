#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Spup.h"

namespace BMW{
namespace Ability{

namespace{
const int anFP[10]={0,30,35,40,45,50,55,60,65,70};
}

int CAbility_Spup::getGetFP(int nAttr)const
{
	if(nAttr<0) return 0;
	return anFP[nAttr];
}

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Spup::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().setSP(slg.getBattle().getMaxSP()+nAttr*6);
}

void CAbility_Spup::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getBattle().setSP(slg.getBattle().getMaxSP()-nAttr*6);
}

} // namespace Ability end
} // namespace BMW end
