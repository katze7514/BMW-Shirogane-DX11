/*
	katze 05/06/29
	精神DB
*/
#pragma once

#include "../Ability/DB/CAbilityDB.h"

namespace BMW{
namespace Spirit{

class CSpiritDB : public Ability::CAbilityDB
{/**
	精神DB
 */
public:
	// ここで、精神コマンドの実体化をする
	void	setAbilityDB(const string& sFile);

	// 効果範囲
	int		getRange(int nID);
};

} // namespace Spirit end
} // namespace BMW end