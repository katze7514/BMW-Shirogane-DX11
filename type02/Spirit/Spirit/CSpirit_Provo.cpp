#include "stdafx.h"

#include "../../Ability/IDAbility.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Provo.h"

namespace BMW{
namespace Spirit{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
void CSpirit_Provo::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
#ifdef BMW_DEBUG
	CDbg().Out("ProvoSpirit %d %d",slg.getID(),nAttr);
#endif
	// 適用
	Chara::CValidSpirit& spirit = slg.getBattle().getValidSpirit();
	spirit.valid(true,Chara::CValidSpirit::PROVO);
	// nAttrに挑発を使ったキャラのSLG IDを入れておく
	spirit.setProvoID(nAttr);
}

void CSpirit_Provo::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 適用
	Chara::CValidSpirit& spirit = slg.getBattle().getValidSpirit();
	spirit.valid(false,Chara::CValidSpirit::PROVO);
	spirit.setProvoID(-1);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Provo::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Provo::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 二度掛けはできない
	return !slg.getBattle().IsSpirit(Chara::CValidSpirit::PROVO);
}

void CSpirit_Provo::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
