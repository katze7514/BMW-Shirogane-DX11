#include "stdafx.h"

#include "../../Ability/IDAbility.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CSpirit_Supply.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Supply::applyStatus(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	slg.refill(&p,false);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Supply::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

bool CSpirit_Supply::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext)
{
	// 全快の相手には効果無し
	// ENチェック
	if(slg.getBattle().getBattleOffset().getEN()>0) return true;
	// 武器弾数チェック
	const Chara::CDataCharaBattle& battle = slg.getBattle();
	Weapon::CDataWeaponBattle* pWeapon;
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = pContext.getWeaponData(*battle.nextWeapon());
		if(pWeapon==NULL) continue;
		// 弾数だけチェック
		if(pWeapon->getBallet()!=pWeapon->getBalletRest()) return true;
	}
	return false;
}

void CSpirit_Supply::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
