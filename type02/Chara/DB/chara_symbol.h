/*
	katze 05/03/06
	キャラ設定ファイル解析で使うシンボルテーブル
*/
#pragma once

#include "../IDChara.h"
#include "../../Spirit/IDSpirit.h"
#include "../../Ability/IDAbility.h"
#include "../../Item/IDItem.h"

namespace BMW{
namespace Chara{

struct naturesym : public boost::spirit::symbols<int>
{
	naturesym()
	{
		add
			("WEAK",		Chara::Nature::WEAK)
			("NORMAL",		Chara::Nature::NORMAL)
			("STRONG",		Chara::Nature::STRONG)
			("VERYSTRONG",	Chara::Nature::VERYSTRONG)
		;
	}
};

struct growthsym : public boost::spirit::symbols<int>
{
	growthsym()
	{
		add
			("FIGHT_STANDART",			Chara::Growth::FIGHT_STANDART)
			("FIGHT_VERSATILITY",		Chara::Growth::FIGHT_VERSATILITY)
			("FIGHT_DEFENCE",			Chara::Growth::FIGHT_DEFENCE)
			("FIGHT_MAGIC",				Chara::Growth::FIGHT_MAGIC)
			("FIGHT_SLOW",				Chara::Growth::FIGHT_SLOW)
			("MAGIC_STANDART",			Chara::Growth::MAGIC_STANDART)
			("MAGIC_VERSATILITY",		Chara::Growth::MAGIC_VERSATILITY)
			("MAGIC_FIGHT",				Chara::Growth::MAGIC_FIGHT)
			("MAGIC_DEFENCE",			Chara::Growth::MAGIC_DEFENCE)
			("MAGIC_SLOW",				Chara::Growth::MAGIC_SLOW)
			("BACKUP_DEFENCE",			Chara::Growth::BACKUP_DEFENCE)
			("BACKUP_AVOID",			Chara::Growth::BACKUP_AVOID)
			("BACKUP_SP",				Chara::Growth::BACKUP_SP)
			("VERSATILITY_STANDART",	Chara::Growth::VERSATILITY_STANDART)
			("VERSATILITY_SLOW",		Chara::Growth::VERSATILITY_SLOW)
		;
	}
};

struct spiritsym : public boost::spirit::symbols<int>
{
	spiritsym()
	{
		add
			("FIREBALL",	Spirit::FIREBALL)
			("SPIRIT",		Spirit::SPIRIT)
			("AVOID",		Spirit::AVOID)
			("TOUGH",		Spirit::TOUGH)
			("DEFENCE",		Spirit::DEFENCE)
			("CONCENT",		Spirit::CONCENT)
			("HIT",			Spirit::HIT)
			("SYNC",		Spirit::SYNC)
			("ACC",			Spirit::ACC)
			("JUMP",		Spirit::JUMP)
			("AWAKE",		Spirit::AWAKE)
			("AGAIN",		Spirit::AGAIN)
			("GUTS",		Spirit::GUTS)
			("VERYGUTS",	Spirit::VERYGUTS)
			("TRUST",		Spirit::TRUST)
			("FRIEND",		Spirit::FRIEND)
			("SUPPLY",		Spirit::SUPPLY)
			("HOPE",		Spirit::HOPE)
			("POWER",		Spirit::POWER)
			("ENCOURAGE",	Spirit::ENCOURAGE)
			("SNIPE",		Spirit::SNIPE)
			("DIRECT",		Spirit::DIRECT)
			("CHARGE",		Spirit::CHARGE)
			("SPY",			Spirit::SPY)
			("WEAK",		Spirit::WEAK)
			("EASYON",		Spirit::EASYON)
			("MIRACLE",		Spirit::MIRACLE)
			("FORTUNE",		Spirit::FORTUNE)
			("BLESS",		Spirit::BLESS)
			("EFFORT",		Spirit::EFFORT)
			("CHEER",		Spirit::CHEER)
			("FAITH",		Spirit::FAITH)
			("PRAY",		Spirit::PRAY)
			("PROVO",		Spirit::PROVO)
		;
	}
};

struct abilitysym : public boost::spirit::symbols<int>
{
	abilitysym()
	{
		add
			("SPECTER",			Ability::SPECTER)
			("MAGICIAN",		Ability::MAGICIAN)
			("VAMPIRE",			Ability::VAMPIRE)
			("FUNDPOWER",		Ability::FUNDPOWER)
			("COUNTER",			Ability::COUNTER)
			("BACKUPATTACK",	Ability::BACKUPATTACK)
			("BACKUPDEFENCE",	Ability::BACKUPDEFENCE)
			("LINKAGEATTACK",	Ability::LINKAGEATTACK)
			("ATTACKER",		Ability::ATTACKER)
			("REVENGE",			Ability::REVENGE)
			("GUARD",			Ability::GUARD)
			("BREAKLINE",		Ability::BREAKLINE)
			("HITAWAY",			Ability::HITAWAY)
			("MAGICSAVE",		Ability::MAGICSAVE)
			("BATTLESPIRIT",	Ability::BATTLESPIRIT)
			("FIGHTUP",			Ability::FIGHTUP)
			("RITHM",			Ability::RITHM)
			("AGAINST",			Ability::AGAINST)
			("SPUP",			Ability::SPUP)
			("SPRECOVER",		Ability::SPRECOVER)
			("CONCENT",			Ability::CONCENT)
			("LUCKY",			Ability::LUCKY)
			("MAGICBARRIER_A",	Ability::MAGICBARRIER_A)
			("MAGICBARRIER_B",	Ability::MAGICBARRIER_B)
			("MAGICBARRIER_C",	Ability::MAGICBARRIER_C)
			("MAGICRELEASE",	Ability::MAGICRELEASE)
			("AVALON",			Ability::AVALON)
			("ARROWBLESS",		Ability::ARROWBLESS)
			("BATTLEFOLLOW",	Ability::BATTLEFOLLOW)
			("ROAIAS",			Ability::ROAIAS)
			("SUPERPOWER",		Ability::SUPERPOWER)
			("DEMON",			Ability::DEMON)
			("TWELVECROSS",		Ability::TWELVECROSS)
			("DEATH",			Ability::DEATH)
			("ORIGIN",			Ability::ORIGIN)
			("MADRED",			Ability::MADRED)
			("SYNC",			Ability::SYNC)
			("DIVISION",		Ability::DIVISION)
			("DEATH_TRUE",		Ability::DEATH_TRUE)
			("SCAPEGOAT",		Ability::SCAPEGOAT)
			("FUTUREEYE",		Ability::FUTUREEYE)
			("FLY",				Ability::FLY)
			("FUTSUNO",			Ability::FUTSUNO)
			("FLOAT",			Ability::FLOAT)
			("HP_RECOVER_S",	Ability::HP_RECOVER_S)
			("HP_RECOVER_M",	Ability::HP_RECOVER_M)
			("HP_RECOVER_L",	Ability::HP_RECOVER_L)
			("EN_RECOVER_S",	Ability::EN_RECOVER_S)
			("EN_RECOVER_M",	Ability::EN_RECOVER_M)
			("EN_RECOVER_L",	Ability::EN_RECOVER_L)
			("MOVE_UP",			Ability::MOVE_UP)
			("BALLETSAVE",		Ability::BALLETSAVE)
			("IKIYOYO",			Ability::IKIYOYO)
			("ALTER_EGO",		Ability::ALTER_EGO)
			("COND_IGNORE",		Ability::COND_IGNORE)
			("LOLI",			Ability::LOLI)
			("DEKA",			Ability::DEKA)
			("DEATH_EX",		Ability::DEATH_EX)
			("FUTOU",			Ability::FUTOU)
			("FIELD_IGNORE",	Ability::FIELD_IGNORE)
			("KEHAI_SHADAN",	Ability::KEHAI_SHADAN)
			("CHALICE_CONECT",	Ability::CHALICE_CONECT)
			("MEKA_BARRIAR",	Ability::MEKA_BARRIAR)
			("FUTUREEYE_SECOND",Ability::FUTUREEYE_SECOND)
			("TWICE_ACTION",	Ability::TWICE_ACTION)
			("HIYOKURENRI",		Ability::HIYOKURENRI)
			("TRUE_DIVISION",	Ability::TRUE_DIVISION)
			("COLA_BARRIAR",	Ability::COLA_BARRIAR)
			("OPEN_GET",		Ability::OPEN_GET)
			("MUDAI_SHIELD",	Ability::MUDAI_SHIELD)
			("LAND_IGNORE",		Ability::LAND_IGNORE)
			("TWICE_ATTACK",	Ability::TWICE_ATTACK)
			("TWICE_ATTACK_ENEMY",	Ability::TWICE_ATTACK_ENEMY)
			("MEKA_BARRIAR_WEAK",	Ability::MEKA_BARRIAR_WEAK)
		;
	}
};

struct itemsym : public boost::spirit::symbols<int>
{
	itemsym()
	{
		add
			("MEDI",		Item::MEDI)
			("CURRY",		Item::CURRY)
			("SHIRO",		Item::SHIRO)
			("MAGAZINE",	Item::MAGAZINE)
			("MABO",		Item::MABO)
			("OTSU",		Item::OTSU)
			("WOTSU",		Item::WOTSU)
			("NEKO",		Item::NEKO)
			("HOUI",		Item::HOUI)
			("BOOT",		Item::BOOT)
			("KYUDO",		Item::KYUDO)
			("MAID",		Item::MAID)
			("MEGANE",		Item::MEGANE)
			("ROD",			Item::ROD)
			("GEM",			Item::GEM)
			("EARRING",		Item::EARRING)
			("RIBBON",		Item::RIBBON)
			("TIGER",		Item::TIGER)
			("SABER",		Item::SABER)
			("LANCER",		Item::LANCER)
			("ARCHER",		Item::ARCHER)
			("RIDER",		Item::RIDER)
			("CASTER",		Item::CASTER)
			("ASSASSIN",	Item::ASSASSIN)
			("BERSERKER",	Item::BERSERKER)
			("TRUE_ASSASSIN",Item::TRUE_ASSASSIN)
			("GIRU",		Item::GIRU)
			("AVENGER",		Item::AVENGER)
			("BP",			Item::BP)
			("AGONIST",		Item::AGONIST)
			;
	}
};
} // namespace Chara end
} // namespace BMW end