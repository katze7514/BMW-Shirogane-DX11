/*
	katze 08/08/31
	アイテムAGONIST
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Agonist : public Ability::IDataAbility
{/**
	A異常症
	移動力+1、Jump+2、Tough+200、Quick+20、命中+20、射程+1、到達+2
*/
public:
	// コンストラクタ
	CItem_Agonist(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);

	// 武器適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

