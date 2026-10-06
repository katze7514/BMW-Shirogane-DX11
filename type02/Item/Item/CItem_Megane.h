/*
	katze 06/06/12
	アイテムMEGANE
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Megane : public Ability::IDataAbility
{/**
	アイテムMEGANE
*/
public:
	// コンストラクタ
	CItem_Megane(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

