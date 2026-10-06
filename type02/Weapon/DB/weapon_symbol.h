/*
	katze 05/03/06
	ïêäÌDBä÷åWÇÃÉVÉìÉ{Éã
*/
#pragma once

#include "../../Chara/CValidCond.h"
#include "../IDWeapon.h"

namespace BMW{
namespace Weapon{

struct kindsym	: public boost::spirit::symbols<int>
{
	kindsym()
	{
		add
			("FIGHT",			Kind::FIGHT)
			("MAGIC",			Kind::MAGIC)
			("FIGHT_COLLAB",	Kind::FIGHT_COLLAB)
			("MAGIC_COLLAB",	Kind::MAGIC_COLLAB)
			("FIGHT_COND",		Kind::FIGHT_COND)
			("MAGIC_COND",		Kind::MAGIC_COND)
			("STATUS",			Kind::STATUS)
			("CURE",			Kind::CURE)
			("REFILL",			Kind::REFILL)
		;
	}
};

struct costsym : public boost::spirit::symbols<int>
{
	costsym()
	{
		add
			("A",Type::A)
			("B",Type::B)
			("C",Type::C)
			("D",Type::D)
			("E",Type::E)
			("F",Type::F)
			;
	}
};

struct condsym : public boost::spirit::symbols<int>
{
	condsym()
	{
		add
			("ACTION",	Chara::CValidCond::ACTION)
			("MOVE",	Chara::CValidCond::MOVE)
			("DEFENCE",	Chara::CValidCond::DEFENCE)
			("HIT",		Chara::CValidCond::HIT)
			("AVOID",	Chara::CValidCond::AVOID)
			("EN",		Chara::CValidCond::EN)
			("MENTAL",	Chara::CValidCond::MENTAL)
			;
	}
};

struct fieldsym : public boost::spirit::symbols<int>
{
	fieldsym()
	{
		add
			("CENTER",	Field::CENTER)
			("LINE",	Field::LINE)
			("THROW",	Field::THROW)
			;
	}
};

struct specialsym : public boost::spirit::symbols<int>
{
	specialsym()
	{
		add
			("NORMAL",			Special::NORMAL)
			("NORMAL_EXTEND",	Special::NORMAL_EXTEND)
			("COUNTER_EXTEND",	Special::COUNTER_EXTEND)
			("COUNTER_SPECIAL",	Special::COUNTER_SPECIAL)
			;
	}
};

} // namespace Weapon end
} // namespace BMW end