#include "stdafx.h"

#include "../../Ability/IDAbility.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CSpirit_Defence.h"

namespace BMW{
namespace Spirit{
////////////////////////////////////////////
// ステータス適用
////////////////////////////////////////////
void CSpirit_Defence::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを立てる
	slg.getBattle().spirit(true,Chara::CValidSpirit::DEFENCE);
}

void CSpirit_Defence::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// 精神有効フラグを倒す
	slg.getBattle().spirit(false,Chara::CValidSpirit::DEFENCE);
}

/////////////////////////////////////////////
// 補正
/////////////////////////////////////////////
void CSpirit_Defence::applyOffset(SLG::COffsetBattle& data, int nDamage)
{
	data.calcDamage(-(nDamage*3)/4);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CSpirit_Defence::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 二度掛けはできない
	if(slg.getBattle().IsSpirit(Chara::CValidSpirit::DEFENCE)) return false;
	// 現在SPが消費SP(Attr)以上残ってればOK
	// ただし、集中力を持っていたら80%でいい
	if(slg.getBattle().hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	return slg.getBattle().getSP()>=nAttr;
}

void CSpirit_Defence::use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p)
{
	// SPを消費する
	// ただし、集中力を持っていたら80%でいい
	if(battle.hasSkill(Ability::CONCENT)>=0) nAttr=(nAttr*4)/5;
	battle.calcSP(nAttr);
}

} // namespace Spirit end
} // namespace BMW end
