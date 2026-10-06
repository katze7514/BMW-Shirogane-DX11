#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Friend.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Friend::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP全快
	slg.calcHP(-slg.getBattle().getMaxHP());
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Friend::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Friend::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	return slg.getBattle().getBattleOffset().getHP()>0;
}

void CSpirit_Friend::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
