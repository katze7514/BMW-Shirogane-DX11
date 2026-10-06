#include "stdafx.h"

#include "../../SLG/Context/COffsetBattle.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_ChaliceConect.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// 使用可能？
////////////////////////////////////////////
bool CAbility_ChaliceConect::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// ENが足りて気力140以上でダメージ増加
	return slg.getBattle().getEN()>=(nAttr+getEN()) && slg.getBattle().getMental()>=140;
}

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_ChaliceConect::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ1.2倍
	// つまり、補正値に20%分つんでおく
	data.calcDamage(nDamage/5);
}


void CAbility_ChaliceConect::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	if(slg.getBattle().getMental()>=130)
	{
		// HP20%回復
		slg.getBattle().calcHP(-(slg.getBattle().getMaxHP()*2)/10);
		// EN20%回復
		slg.getBattle().calcEN(-(slg.getBattle().getMaxEN()*2)/10);
	}
	else
	{
		// EN10%回復
		slg.getBattle().calcEN(-slg.getBattle().getMaxEN()/10);
	}
}

void CAbility_ChaliceConect::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	if(slg.getBattle().getMental()>=130)
	{
		// HP20%回復
		slg.getBattle().calcHP((slg.getBattle().getMaxHP()*2)/10);
		// EN20%回復
		slg.getBattle().calcEN((slg.getBattle().getMaxEN()*2)/10);
	}
	else
	{
		// EN10%回復
		slg.getBattle().calcEN(slg.getBattle().getMaxEN()/10);
	}
}

} // namespace Ability end
} // namespace BMW end
