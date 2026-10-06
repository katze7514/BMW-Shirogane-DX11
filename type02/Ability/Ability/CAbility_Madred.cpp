#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Madred.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// 補正適用
////////////////////////////////////////////
void CAbility_Madred::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// 4000以下のダメージ無効化
	if(nDamage<=4000) data.calcDamage(-4000);
}
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Madred::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力100以上、ENを5消費する
	return slg.getBattle().getMental()>=100 && (slg.getBattle().getEN()>=getEN()+nAttr);
}

} // namespace Ability end
} // namespace BMW end
