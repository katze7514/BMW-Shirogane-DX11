#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "../IDSpirit.h"
#include "CSpirit_Pray.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Pray::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 状態変化フラグを全部倒す
	slg.getBattle().getValidCond().reset();
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Pray::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Pray::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext)
{
	// 対象が状態変化が掛かってる
	const Chara::CDataCharaBattle& battle = slg.getBattle();
	for(int i=Chara::CValidCond::ACTION; i<=Chara::CValidCond::AVOID; ++i)
		if(battle.IsCond(i)) return true;

	return false;
}

void CSpirit_Pray::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
