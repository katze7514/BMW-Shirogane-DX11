#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_TwiceAttack.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_TwiceAttack::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN足りて気力120以上で使用可能
	return slg.getBattle().getMental()>=120;
}

} // namespace Ability end
} // namespace BMW end
