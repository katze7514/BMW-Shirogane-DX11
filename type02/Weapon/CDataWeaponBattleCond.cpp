#include "stdafx.h"

#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "CDataWeaponBattleCond.h"

namespace BMW{
namespace Weapon{

void CDataWeaponBattleCond::apply(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context)
{// フェーズによって、相手の持続ターンを変更する
	// 信念が掛かっていたら無効
	// もしくは、タイガーストラップを装備してる
	// もしくは状態変化無効技能を持つ相手だったら無効
	if(chara.getBattle().IsSpirit(Chara::CValidSpirit::FAITH)
	|| chara.getBattle().IsHasItem(Item::TIGER)
	|| chara.getBattle().IsTalent(Ability::COND_IGNORE)) return;

	if(getCond()==Chara::CValidCond::EN)
	{// EN減少
		chara.getBattle().calcEN(getCondValue());
	}
	ef(getCond()==Chara::CValidCond::MENTAL)
	{// 気力減少
		chara.getBattle().calcMental(getCondValue());
	}
	else
	{
		// 同じ効果は重複しない
		if(chara.getBattle().IsCond(getCond())) return;
		// ターン数が入ってるので×3
		// 実際にはフェーズ毎にデクリメントされる
		//if(getCond()!=Chara::CValidCond::ACTION)
			chara.getBattle().cond(getCondValue()*3,getCond());
		//else// この後で-3されるから、行動不能の時は＋3
		//	chara.getBattle().cond(getCondValue()*3,getCond());

	}
}

} // namespace Weapon end
} // namespace BMW end