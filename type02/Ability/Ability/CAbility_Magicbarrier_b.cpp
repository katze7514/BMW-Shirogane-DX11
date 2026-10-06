#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Magicbarrier_b.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Magicbarrier_b::applyOffset(SLG::COffsetBattle& data)
{
	// ダメージ800軽減
	data.calcDamage(-800);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Magicbarrier_b::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN消費なので
	return slg.getBattle().getEN()>=getEN()+nAttr;
}

} // namespace Ability end
} // namespace BMW end
