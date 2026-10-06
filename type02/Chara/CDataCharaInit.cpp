#include "stdafx.h"

#include "IDChara.h"
#include "CDataCharaBase.h"
#include "CDataCharaInit.h"

namespace BMW{
namespace Chara{

void CDataCharaInit::apply(CDataCharaBase* base, bool bCont)
{// データの設定
	base->setID(getID());
	base->setFaceID(getFaceID());
	base->setMapSymbolID(getMapSymbolID());
	base->setDemoID(getDemoID());
	base->setSymbolID(getSymbolID());
	base->setBgmID(getBgmID());

	base->setChara(getChara());
	base->setGrowth(getGrowth());
	base->setPena(getPena());
	base->setFP(getFP());
	base->setFund(getFund());
	base->setBattle(getBattle());

	// 固有能力
	base->clearTalent();
	beginTalent();
	while(!endTalent())	base->addTalent(*nextTalent());

	// 技能フラグ
	// ↑は、GrowthAbilityで設定される
	//base->setSkillValid(getSkillValid());

	if(!bCont)
	{// コンテニュー時のデータ生成ならいらない
		// 武器
		base->clearWeapon();
		beginWeapon();
		while(!endWeapon()) base->addWeapon(*nextWeapon());
	}

	base->setWeaponCost(getWeaponCost());
	base->setItemMax(getItemMax());
}

void CDataCharaInit::applySub(CDataCharaBase* base,bool bCont)
{// 差分として適用
	// キャラIDは上書き
	base->setID(getID());

	// 0以下でなければ能力値系は上書き
	if(getFaceID()>0)				base->setFaceID(getFaceID());
	if(!getMapSymbolID().empty())	base->setMapSymbolID(getMapSymbolID());
	if(!getDemoID().empty())		base->setDemoID(getDemoID());
	if(!getSymbolID().empty())		base->setSymbolID(getSymbolID());
	if(getBgmID()>0)				base->setBgmID(getBgmID());

	if(getChara()>0)				base->setChara(getChara());
	if(getGrowth()>0)				base->setGrowth(getGrowth());
	if(getPena()>0)					base->setPena(getPena());
	if(getFP()>0)					base->setFP(getFP());

	base->setFundSub(getFund());
	base->setBattleSub(getBattle());

	// 固有能力は追加
	beginTalent();
	while(!endTalent())	base->addTalent(*nextTalent());

	// 武器も追加
	if(!bCont)
	{// コンテニュー時のデータ生成ならいらない
		// 武器
		beginWeapon();
		while(!endWeapon()) base->addWeapon(*nextWeapon());
	}

	if(getWeaponCost()>0)	base->setWeaponCost(getWeaponCost());
	if(getItemMax()>0)		base->setItemMax(getItemMax());
}

} // namespace Chara end
} // namespace BMW end