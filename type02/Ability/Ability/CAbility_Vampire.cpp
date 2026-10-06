#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Vampire.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Vampire::applyOffset(SLG::COffsetHit& data, int nAttr)
{
	const CVampireTableBase& vmp = vampireTable_.getTable(nAttr-1);
	data.calcHit(vmp.getHit());
	data.calcAvoid(vmp.getAvoid());
}

void CAbility_Vampire::applyOffset(SLG::COffsetBattle& data, int nAttr, SLG::CDataCharaSLG& slg)
{
	const CVampireTableBase& vmp = vampireTable_.getTable(nAttr-1);
	data.calcCT(vmp.getCT());
	data.calcTough(slg.getBattle().getTough()*vmp.getDef()/100);
}

} // namespace Ability end
} // namespace BMW end
