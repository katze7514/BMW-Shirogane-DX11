/*
	katze 06/06/12
	アイテムROD
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Rod : public Ability::IDataAbility
{/**
	ファンタズムロッド
	CT+10
*/
public:
	// コンストラクタ
	CItem_Rod(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

