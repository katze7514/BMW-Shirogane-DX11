#include "stdafx.h"

#include "../../SLG/Context/COffsetBattle.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Demon.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Demon::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ1.25倍
	// 補正分の25%を積んでおく
	data.calcDamage(nDamage/4);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Demon::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上で発動
	return slg.getBattle().getMental()>=130;
}

} // namespace Ability end
} // namespace BMW end
