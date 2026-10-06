#include "stdafx.h"

#include "../Ability/IDAbility.h"

#include "CValidSkill.h"

namespace BMW{
namespace Chara{

void CValidSkill::decValid(int nID)
{
	switch(nID)
	{
	case Ability::FUNDPOWER:	--naValid_[FUNDPOWER];		break;
	case Ability::COUNTER:		--naValid_[COUNTER];		break;
	case Ability::BACKUPATTACK:	--naValid_[BACKUPATTACK];	break;
	case Ability::BACKUPDEFENCE:--naValid_[BACKUPDEFENCE];	break;
	case Ability::SPUP:			--naValid_[SPUP];			break;
	case Ability::MOVE_UP:		--naValid_[MOVE_UP];		break;
	default: break;
	}
}

} // namespace Chara end
} // namespace BMW end