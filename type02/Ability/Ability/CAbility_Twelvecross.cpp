#include "stdafx.h"

#include "../../mode.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Twelvecross.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CAbility_Twelvecross::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	// ダメージ5000以下無効化
	const int nDown =
	#ifdef HP_DIV
		500
	#else
		5000
	#endif
	;
	if(nDamage<=nDown) data.calcDamage(-nDown);
	// 5000越えても1500軽減
	else data.calcDamage(-1500);
}

} // namespace Ability end
} // namespace BMW end
