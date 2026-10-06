/*
	katze 08/06/17
	CONCENT
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_TwiceAction : public IDataAbility
{/**
	二回行動
*/
public:
	// コンストラクタ
	CAbility_TwiceAction(const string& sGuiDefID):IDataAbility(sGuiDefID){}
};

} // namespace Ability end
} // namespace BMW end

