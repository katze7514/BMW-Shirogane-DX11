#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Spy.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Spy::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getState().apper(true);
}

void CSpirit_Spy::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	slg.getState().apper(false);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Spy::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	if(nAttr<=0) nAttr=1;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Spy::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// すでに出現状態か？
	return !slg.getState().IsApper();
}

void CSpirit_Spy::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	if(nAttr<=0) nAttr=1;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
