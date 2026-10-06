#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Effort.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Effort::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを立てる
	slg.getBattle().spirit(true,Chara::CValidSpirit::EFFORT);
}

void CSpirit_Effort::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを倒す
	slg.getBattle().spirit(false,Chara::CValidSpirit::EFFORT);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Effort::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 二度掛けはできない
	if(slg.getBattle().IsSpirit(Chara::CValidSpirit::EFFORT)) return false;
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

void CSpirit_Effort::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
