#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/IDSLG.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Action/IAction.h"

#include "CSpirit_Again.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Again::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// もう一回動ける
	slg.getState().setAct(SLG::Act::BEFORE);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Again::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Again::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// NPCには掛けられない
	return !slg.getAction()->IsNonPlayer() && slg.getState().getAct()!=SLG::Act::BEFORE;
}

void CSpirit_Again::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
