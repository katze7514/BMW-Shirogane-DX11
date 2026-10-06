/*
	katze 05/07/08
	アイテムDB
*/
#pragma once

#include "../Ability/DB/CAbilityDB.h"

namespace BMW{
namespace Item{

class CItemDB : public Ability::CAbilityDB
{/**
	精神DB
 */
public:
	// ここで、精神コマンドの実体化をする
	void	setAbilityDB(const string& sFile);

	// 効果範囲
	bool	IsUse(int nID);
	bool	IsStatus(int nID);
	bool	IsWeapon(int nID);
};

} // namespace Item end
} // namespace BMW end