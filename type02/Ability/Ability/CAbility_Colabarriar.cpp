#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Colabarriar.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Colabarriar::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ3000以下無効化
	if(nDamage<=3000) data.calcDamage(-3000);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Colabarriar::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN消費
	return slg.getBattle().getEN()>=getEN()+nAttr;
}

} // namespace Ability end
} // namespace BMW end
