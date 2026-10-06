#include "stdafx.h"

//#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Attacker.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Attacker::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ1.2倍
	// つまり、補正値に20%分つんでおく
	data.calcDamage(nDamage/5);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Attacker::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上
	return slg.getBattle().getMental()>=130;
}

} // namespace Ability end
} // namespace BMW end
