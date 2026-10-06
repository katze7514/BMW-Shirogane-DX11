/*
	katze 04/11/13
	ƒLƒƒƒ‰‚ÉŠÖ‚·‚éID
*/
#pragma once

namespace BMW{
namespace Chara{

namespace Nature{
// «Šiƒ^ƒCƒv
enum eNatureType{
	WEAK,		// Žã‹C
	NORMAL,		// •’Ê
	STRONG,		// ‹­‹C
	VERYSTRONG,	// ’´‹­‹C
};

} // namespace Nature end

namespace Growth{
// ¬’·ƒ^ƒCƒv
enum eGrowthType{
	FIGHT_STANDART,			// Ši“¬Œn•W€
	FIGHT_VERSATILITY,		// Ši“¬Œn–œ”\
	FIGHT_DEFENCE,			// Ši“¬Œn–hŒä
	FIGHT_MAGIC,			// Ši“¬Œn–‚—Í
	FIGHT_SLOW,				// Ši“¬Œn‘åŠí”Ó¬
	MAGIC_STANDART,			// –‚pŒn•W€
	MAGIC_VERSATILITY,		// –‚pŒn–œ”\
	MAGIC_FIGHT,			// –‚pŒnŠi“¬
	MAGIC_DEFENCE,			// –‚pŒn–hŒä
	MAGIC_SLOW,				// –‚pŒn‘åŠí”Ó¬
	BACKUP_DEFENCE,			// ‰‡ŒìŒn–hŒä
	BACKUP_AVOID,			// ‰‡ŒìŒn‰ñ”ð
	BACKUP_SP,				// ‰‡ŒìŒnSP
	VERSATILITY_STANDART,	// –œ”\Œn•W€
	VERSATILITY_SLOW,		// –œ”\Œn‘åŠí”Ó¬
};
} // namespace Growth end

} // namespace Chara end
} // namespace BMW end