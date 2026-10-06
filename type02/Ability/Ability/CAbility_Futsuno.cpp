#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Futsuno.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// 補正適用
////////////////////////////////////////////
void CAbility_Futsuno::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// 1500以下のダメージ無効化
	if(nDamage<=1500) data.calcDamage(-1500);
}
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Futsuno::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// ENを5消費する
	return slg.getBattle().getEN()>=getEN()+nAttr;
}

} // namespace Ability end
} // namespace BMW end
