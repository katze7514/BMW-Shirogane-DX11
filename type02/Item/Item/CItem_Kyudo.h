/*
	katze 06/06/12
	アイテムKYUDO
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Kyudo : public Ability::IDataAbility
{/**
	弓道着
	射程+1、到達+2
*/
public:
	// コンストラクタ
	CItem_Kyudo(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
	void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr);
};

} // namespace Item end
} // namespace BMW end

