#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetWeapon.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Futou.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// 補正
////////////////////////////////////////////
void CAbility_Futou::applyOffset(SLG::COffsetHit& data)
{
	data.calcHit(30);
}

void CAbility_Futou::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージを10%にするので、軽減分を設定
	data.calcDamage(-(nDamage*9)/10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Futou::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上かつピンチ！
	return slg.getBattle().getMental()>=130 && slg.getState().IsPinch();
}

} // namespace Ability end
} // namespace BMW end
