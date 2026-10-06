/*
	katze 06/06/12
	アイテムTRUE_ASSASSIN
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_True_assassin : public Ability::IDataAbility
{/**
	真アサシンカード
	HIT+30、CT+20
*/
public:
	// コンストラクタ
	CItem_True_assassin(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

