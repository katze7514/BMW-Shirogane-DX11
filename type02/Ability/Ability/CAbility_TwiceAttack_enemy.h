/*
	katze 08/01/05
	二回攻撃/一気呵成
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_TwiceAttack_enemy : public IDataAbility
{/**
	二回攻撃/一気呵成

	敵バージョンには気力制限が無い
*/
public:
	// コンストラクタ
	CAbility_TwiceAttack_enemy(const string& sGuiDefID):IDataAbility(sGuiDefID){}
};

} // namespace Ability end
} // namespace BMW end

