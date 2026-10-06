/*
	katze 06/06/12
	アイテムARCHER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Archer : public Ability::IDataAbility
{/**
	アーチャーのカード
	射程+2、到達+4、命中+20
*/
public:
	// コンストラクタ
	CItem_Archer(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

