#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Guard.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// 補正
////////////////////////////////////////////
void CAbility_Guard::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージを80%にするので、軽減分を設定
	data.calcDamage(-nDamage/5);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Guard::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上
	return slg.getBattle().getMental()>=130;
}

} // namespace Ability end
} // namespace BMW end
