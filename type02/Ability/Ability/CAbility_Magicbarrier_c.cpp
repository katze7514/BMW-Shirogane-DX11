#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Magicbarrier_c.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Magicbarrier_c::applyOffset(SLG::COffsetBattle& data)
{
	// ダメージ600軽減
	data.calcDamage(-600);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Magicbarrier_c::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN消費
	return slg.getBattle().getEN()>=getEN()+nAttr;
}

} // namespace Ability end
} // namespace BMW end
