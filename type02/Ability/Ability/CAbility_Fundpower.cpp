#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Fundpower.h"

namespace BMW{
namespace Ability{
namespace{
const int anFP[10]={0,20,25,30,35,40,45,50,55,60};
}
int CAbility_Fundpower::getGetFP(int nAttr)const
{
	if(nAttr<0) return 0;
	return anFP[nAttr];
}

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Fundpower::applyOffset(SLG::COffsetHit& data, int nAttr, const SLG::CDataCharaSLG& base)
{
	const CFundTableBase& fund = fundTable_.getTable(nAttr-1,base.getBattle());
	data.calcHit(fund.getHit());
	data.calcAvoid(fund.getAvoid());
}

void CAbility_Fundpower::applyOffset(SLG::COffsetBattle& data, int nAttr, const SLG::CDataCharaSLG& base)
{
	const CFundTableBase& fund = fundTable_.getTable(nAttr-1,base.getBattle());
	data.calcCT(fund.getCT());
	// 値%Toughが上がるので、それを設定
	data.calcTough(base.getBattle().getTough()*fund.getDef()/100);
}

} // namespace Ability end
} // namespace BMW end
