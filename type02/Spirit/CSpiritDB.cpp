#include "stdafx.h"

#include "../GUI/DB/CGuiDefDB.h"

#include "IDSpirit.h"
#include "Spirit.h"
#include "CSpiritDB.h"

namespace BMW{
namespace Spirit{

void CSpiritDB::setAbilityDB(const string& sFile)
{
	pGuiDef_->setGuiDef(sFile);

	// FIREBALL
	addData(new CSpirit_Fireball("FIREBALL"),FIREBALL);

	// SPIRIT
	addData(new CSpirit_Spirit("SPIRIT"),SPIRIT);

	// AVOID
	addData(new CSpirit_Avoid("AVOID"),AVOID);

	// TOUGH
	addData(new CSpirit_Tough("TOUGH"),TOUGH);

	// DEFENCE
	addData(new CSpirit_Defence("DEFENCE"),DEFENCE);

	// CONCENT
	addData(new CSpirit_Concent("CONCENT"),CONCENT);

	// HIT
	addData(new CSpirit_Hit("HIT"),HIT);

	// SYNC
	addData(new CSpirit_Sync("SYNC"),SYNC);

	// ACC
	addData(new CSpirit_Acc("ACC"),ACC);

	// JUMP
	addData(new CSpirit_Jump("JUMP"),JUMP);

	// AWAKE
	addData(new CSpirit_Awake("AWAKE"),AWAKE);

	// AGAIN
	addData(new CSpirit_Again("AGAIN"),AGAIN);

	// GUTS
	addData(new CSpirit_Guts("GUTS"),GUTS);

	// VERYGUTS
	addData(new CSpirit_Veryguts("VERYGUTS"),VERYGUTS);

	// TRUST
	addData(new CSpirit_Trust("TRUST"),TRUST);

	// FRIEND
	addData(new CSpirit_Friend("FRIEND"),FRIEND);

	// SUPPLY
	addData(new CSpirit_Supply("SUPPLY"),SUPPLY);

	// HOPE
	addData(new CSpirit_Hope("HOPE"),HOPE);

	// POWER
	addData(new CSpirit_Power("POWER"),POWER);

	// ENCOURAGE
	addData(new CSpirit_Encourage("ENCOURAGE"),ENCOURAGE);

	// SNIPE
	addData(new CSpirit_Snipe("SNIPE"),SNIPE);

	// DIRECT
	addData(new CSpirit_Direct("DIRECT"),DIRECT);

	// CHARGE
	addData(new CSpirit_Charge("CHARGE"),CHARGE);

	// SPY
	addData(new CSpirit_Spy("SPY"),SPY);

	// WEAK
	addData(new CSpirit_Weak("WEAK"),WEAK);

	// EASYON
	addData(new CSpirit_Easyon("EASYON"),EASYON);

	// MIRACLE
	addData(new CSpirit_Miracle("MIRACLE"),MIRACLE);

	// FORTUNE
	addData(new CSpirit_Fortune("FORTUNE"),FORTUNE);

	// BLESS
	addData(new CSpirit_Bless("BLESS"),BLESS);

	// EFFORT
	addData(new CSpirit_Effort("EFFORT"),EFFORT);

	// CHEER
	addData(new CSpirit_Cheer("CHEER"),CHEER);

	// FAITH
	addData(new CSpirit_Faith("FAITH"),FAITH);

	// PRAY
	addData(new CSpirit_Pray("PRAY"),PRAY);

	// PROVO
	addData(new CSpirit_Provo("PROVO"),PROVO);
}

int CSpiritDB::getRange(int nID)
{
	switch(nID)
	{
	case SYNC:
	case AGAIN:
	case TRUST:
	case FRIEND:
	case SUPPLY:
	case HOPE:
	case ENCOURAGE:
	case BLESS:
	case CHEER:
	case PRAY:
		return Range::FRIEND;

	case SPY:
	case WEAK:
	case PROVO:
		return Range::ENEMY;

	default: return Range::SELF;
	}
}

} // namespace Spirit end
} // namespace BMW end