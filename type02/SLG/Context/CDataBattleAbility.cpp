#include "stdafx.h"

#include "../../Ability/IDAbility.h"
#include "CDataBattleAbility.h"

namespace BMW{
namespace SLG{

bool CDataBattleAbility::IsBarriar()
{// バリア技能発動？
	return IsAbility(Ability::MADRED)
		|| IsAbility(Ability::MAGICBARRIER_A)
		|| IsAbility(Ability::MAGICBARRIER_B)
		|| IsAbility(Ability::MAGICBARRIER_C)
		|| IsAbility(Ability::ROAIAS)
		|| IsAbility(Ability::TWELVECROSS)
		|| IsAbility(Ability::FUTSUNO)
		|| IsAbility(Ability::MUDAI_SHIELD)
		|| IsAbility(Ability::MEKA_BARRIAR)
		|| IsAbility(Ability::COLA_BARRIAR)
		|| IsAbility(Ability::MEKA_BARRIAR_WEAK)
		;
}

bool CDataBattleAbility::IsAlterEgo()
{// 分身技能発動？
	return IsAbility(Ability::FUTUREEYE)
		|| IsAbility(Ability::DEATH_TRUE)
		|| IsAbility(Ability::ALTER_EGO)
		|| IsAbility(Ability::OPEN_GET)
		|| IsAbility(Ability::AVALON)
		;
}

} // namespace SLG end
} // namespace BMW end