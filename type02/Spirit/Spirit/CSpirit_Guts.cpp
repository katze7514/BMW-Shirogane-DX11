#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "../IDSpirit.h"
#include "CSpirit_Guts.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Guts::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HPを30%回復する
	slg.calcHP(-(slg.getBattle().getMaxHP()*3)/10);
}

void CSpirit_Guts::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HPを30%回復する
	slg.calcHP(slg.getBattle().getMaxHP()*3/10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Guts::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// HP全快だったら意味なし
	if(slg.getBattle().getBattleOffset().getHP()==0) return false;

	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

void CSpirit_Guts::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
