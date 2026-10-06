#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Roaias.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// 補正
////////////////////////////////////////////
void CAbility_Roaias::applyOffset(SLG::COffsetBattle& data)
{
	// ダメージ1000軽減
	// 相手の武器がT属性だったら3000軽減
	data.calcDamage(-1000);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Roaias::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力120以上でEN足りる
	return slg.getBattle().getMental()>=120 && slg.getBattle().getEN()>=getEN();
}

} // namespace Ability end
} // namespace BMW end
