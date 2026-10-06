#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Power.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Power::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+10
	slg.getBattle().calcMental(10);
}

void CSpirit_Power::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力-10
	slg.getBattle().calcMental(-10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Power::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力MAXだったら意味無し
	if(slg.getBattle().getMental()==MENTAL_MAX) return false;

	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

void CSpirit_Power::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
