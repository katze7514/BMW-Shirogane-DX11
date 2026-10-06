#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Encourage.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Encourage::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+10
	slg.getBattle().calcMental(10);
}

void CSpirit_Encourage::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 気力+10
	slg.getBattle().calcMental(-10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Encourage::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{	
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Encourage::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{	
	// 対象の気力がMAXだったら意味無し
	return slg.getBattle().getMental()!=MENTAL_MAX;
}

void CSpirit_Encourage::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
