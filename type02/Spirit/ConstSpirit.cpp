#include "stdafx.h"

#include "IDSpirit.h"
#include "ConstSpirit.h"

namespace BMW{
namespace Spirit{

CSpiritName::CSpiritName()
{
	writeMap(NO,			"--------");
	writeMap(FIREBALL,		"”MŒŒ");
	writeMap(SPIRIT,		"°");
	writeMap(AVOID,			"‚Ğ‚ç‚ß‚«");
	writeMap(TOUGH,			"•s‹ü");
	writeMap(DEFENCE,		"“S•Ç");
	writeMap(CONCENT,		"W’†");
	writeMap(HIT,			"•K’†");
	writeMap(SYNC,			"Š´‰");
	writeMap(ACC,			"‰Á‘¬");
	writeMap(JUMP,			"’µ–ô");
	writeMap(AWAKE,			"ŠoÁ");
	writeMap(AGAIN,			"Ä“®");
	writeMap(GUTS,			"ª«");
	writeMap(VERYGUTS,		"ƒhª«");
	writeMap(TRUST,			"M—Š");
	writeMap(FRIEND,		"—Fî");
	writeMap(SUPPLY,		"•â‹‹");
	writeMap(HOPE,			"Šú‘Ò");
	writeMap(POWER,			"‹C‡");
	writeMap(ENCOURAGE,		"Œƒ—ã");
	writeMap(SNIPE,			"‘_Œ‚");
	writeMap(DIRECT,		"’¼Œ‚");
	writeMap(CHARGE,		"“ËŒ‚");
	writeMap(SPY,			"’ã@");
	writeMap(WEAK,			"’E—Í");
	writeMap(EASYON,		"‚Ä‚©‚°‚ñ");
	writeMap(MIRACLE,		"ŠïÕ");
	writeMap(FORTUNE,		"K‰^");
	writeMap(BLESS,			"j•Ÿ");
	writeMap(EFFORT,		"“w—Í");
	writeMap(CHEER,			"‰‰‡");
	writeMap(FAITH,			"M”O");
	writeMap(PRAY,			"‹F‚è");
	writeMap(PROVO,			"’§”­");
}

CSpiritName Const::spiritName_;

} // namespace Spirit end
} // namespace BMW end