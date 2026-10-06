#include "stdafx.h"

#include "CAbility_Backupattack.h"

namespace BMW{
namespace Ability{

namespace{
const int anFP[5]={0,50,60,70,80};
}

int CAbility_Backupattack::getGetFP(int nAttr)const
{
	return anFP[nAttr];
}

} // namespace Ability end
} // namespace BMW end
