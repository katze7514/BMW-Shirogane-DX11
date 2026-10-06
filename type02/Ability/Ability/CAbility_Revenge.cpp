#include "stdafx.h"

#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Revenge.h"

namespace BMW{
namespace Ability{

////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Revenge::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ1.2倍
	data.calcDamage(nDamage/5);
}

} // namespace Ability end
} // namespace BMW end
