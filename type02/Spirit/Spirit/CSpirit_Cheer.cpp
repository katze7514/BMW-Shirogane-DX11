#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Cheer.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Cheer::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを立てる
	slg.getBattle().spirit(true,Chara::CValidSpirit::EFFORT);
}

void CSpirit_Cheer::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを倒す
	slg.getBattle().spirit(false,Chara::CValidSpirit::EFFORT);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Cheer::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Cheer::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 二度掛けはできない
	return !slg.getBattle().IsSpirit(Chara::CValidSpirit::EFFORT);
}

void CSpirit_Cheer::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
