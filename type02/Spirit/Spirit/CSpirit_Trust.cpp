#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Trust.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Trust::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP30%回復
	slg.calcHP(-(slg.getBattle().getMaxHP()*3)/10);
}

void CSpirit_Trust::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP30%回復
	slg.calcHP(slg.getBattle().getMaxHP()*3/10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Trust::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Trust::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 相手のHPが全快だったら意味無し
	return slg.getBattle().getBattleOffset().getHP()>0;
}

void CSpirit_Trust::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
