/*
	katze 05/06/29
	SLGƒVƒ“ƒ{ƒ‹
*/
#pragma once

#include "../../../Hero/IDHero.h"
#include "../../../Chara/CValidCond.h"

#include "../../../BMW/IDBMW.h"

#include "../../IDSLG.h"
#include "../../Action/IDAction.h"
#include "../../Context/CCode_chara.h"
#include "../../Sally/CSally_add_chara_map.h"
#include "../../Sally/CSally_del_chara_map.h"
#include "../../Sally/CSally_change_chara.h"
#include "../../Victory/CVictory_change.h"

namespace BMW{
namespace SLG{

struct factory_symbol : public boost::spirit::symbols<>
{
	factory_symbol()
	{
		add
			("C_06", BMW::Scenario::SLG::C_06)
			("C_11", BMW::Scenario::SLG::C_11)
			("T_20", BMW::Scenario::SLG::T_20)
			;
	}
};

struct phase_symbol : public boost::spirit::symbols<>
{
	phase_symbol()
	{
		add
			("PLAYER",Phase::PLAYER)
			("ENEMY",Phase::ENEMY)
			("NEUTRAL",Phase::NEUTRAL)
			;
	}
};

struct action_symbol : public boost::spirit::symbols<>
{
	action_symbol()
	{
		add
			("PLAYER",		Action::PLAYER)
			("NORMAL",		Action::NORMAL)
			("WAIT",		Action::WAIT)
			("BATTERY",		Action::BATTERY)
			("WALL",		Action::WALL)
			("WALL_COUNTER",Action::WALL_COUNTER)
			("NORMAL_PARAM",Action::NORMAL_PARAM)
			("NORMAL_EVAL", Action::NORMAL_EVAL)
			("FIELD_PARAM",	Action::FIELD_PARAM)
			("FIELD_EVAL", Action::FIELD_EVAL)
			("AKABINE",		Action::AKABINE)
			("NO",			Action::NOACT)
			;
	}
};

struct action_move_symbol : public boost::spirit::symbols<>
{
	action_move_symbol()
	{
		add
			("MOVE",Action::MOVE)
			("MOVE_NO",Action::MOVE_NO)
			("MOVE_ATK",Action::MOVE_ATK)
			("MOVE_BATTERY",Action::MOVE_BATTERY)
			("MOVE_ONLY",Action::MOVE_ONLY)
			("MOVE_ONLY_ATK",Action::MOVE_ONLY_ATK)
			;
	}
};

struct action_evalmove_symbol : public boost::spirit::symbols<>
{
	action_evalmove_symbol()
	{
		add
			("SHORT",Action::SHORT)
			("LONG",Action::LONG)
			;
	}
};

struct action_evalhp_symbol : public boost::spirit::symbols<>
{
	action_evalhp_symbol()
	{
		add
			("LOW",Action::LOW)
			("HIGH",Action::HIGH)
			;
	}
};

struct way_symbol : public boost::spirit::symbols<>
{
	way_symbol()
	{
		add
			("TOP",Way::TOP)
			("LEFT",Way::LEFT)
			("BOTTOM",Way::BOTTOM)
			("RIGHT",Way::RIGHT)
			;
	}
};

struct add_effect_symbol : public boost::spirit::symbols<>
{
	add_effect_symbol()
	{
		add
			("NO",Sally::CSally_add_chara_map::NO)
			("NORMAL",Sally::CSally_add_chara_map::NORMAL)
			("BOSS",Sally::CSally_add_chara_map::BOSS)
			("WARAKIA",Sally::CSally_add_chara_map::WARAKIA)
			;
	}
};

struct del_effect_symbol : public boost::spirit::symbols<>
{
	del_effect_symbol()
	{
		add
			("NO",Sally::CSally_del_chara_map::NO)
			("DEATH",Sally::CSally_del_chara_map::DEATH)
			("REMOVE",Sally::CSally_del_chara_map::REMOVE)
			;
	}
};

struct chara_kind_symbol : public boost::spirit::symbols<>
{
	chara_kind_symbol()
	{
		add
			("HP",Code::CCode_chara::HP)
			("EN",Code::CCode_chara::EN)
			("MENTAL",Code::CCode_chara::MENTAL)
			("STATE",Code::CCode_chara::STATE)
			("ACT",Code::CCode_chara::ACT)
			("WAY",Code::CCode_chara::WAY)
			("APPER",Code::CCode_chara::APPER)
			("VALID",Code::CCode_chara::VALID)
			("ACTION",Code::CCode_chara::ACTION)
			("LOVE",Code::CCode_chara::LOVE)
			("SP",Code::CCode_chara::SP)
			;
	}
};

struct chara_valuetype_symbol : public boost::spirit::symbols<>
{
	chara_valuetype_symbol()
	{
		add
			("ABS",Code::CCode_chara::ABS)
			("RATIO_MAX",Code::CCode_chara::RATIO_MAX)
			("RATIO_CURRENT",Code::CCode_chara::RATIO_CURRENT)
			;
	}
};

struct chara_validtype_symbol : public boost::spirit::symbols<>
{
	chara_validtype_symbol()
	{
		add
			("MOVE",Code::CCode_chara::MOVE)
			("ATTACK",Code::CCode_chara::ATTACK)
			("SPIRIT",Code::CCode_chara::SPIRIT)
			("ITEM",Code::CCode_chara::ITEM)
			("CURE",Code::CCode_chara::CURE)
			("REFILL",Code::CCode_chara::REFILL)
			("PERS",Code::CCode_chara::PERS)
			;
	}
};

struct chara_statetype_symbol : public boost::spirit::symbols<>
{
	chara_statetype_symbol()
	{
		add
			("ACTION",	Chara::CValidCond::ACTION)
			("MOVE",	Chara::CValidCond::MOVE)
			("DEFENCE",	Chara::CValidCond::DEFENCE)
			("HIT",		Chara::CValidCond::HIT)
			("AVOID",	Chara::CValidCond::AVOID)
			;
	}
};

struct chara_actvalue_symbol : public boost::spirit::symbols<>
{
	chara_actvalue_symbol()
	{
		add
			("BEFORE",		Act::BEFORE)
			("AFTER",		Act::AFTER)
			("DEATH",		Act::DEATH)
			("DEATH_EVENT",	Act::DEATH_EVENT)
			("REMOVE",		Act::REMOVE)
			("REMOVE_EVENT",-1)
			;
	}
};

struct expert_symbol : public boost::spirit::symbols<>
{
	expert_symbol()
	{
		add
			("NORMAL",Expert::NORMAL)
			("HARD",Expert::HARD)
			("HEAVY", Expert::HEAVY)
			("HELL", Expert::HELL)
			("ALL_BEFORE", Expert::ALL_BEFORE)
			("ALL_AFTER", Expert::ALL_AFTER)
			("OVER_45", Expert::OVER_45)
		;
	}
};

struct hero_symbol : public boost::spirit::symbols<>
{
	hero_symbol()
	{
		add
			("TAKUMI", Hero::Target::TAKUMI)
			("HARUNA",Hero::Target::HARUNA)
		;
	}
};

struct vic_symbol : public boost::spirit::symbols<>
{
	vic_symbol()
	{
		add
			("VICTORY", Victory::CVictory_change::VICTORY)
			("LOSE",Victory::CVictory_change::LOSE)
			("EXPERT",Victory::CVictory_change::EXPERT)
		;
	}
};

struct lv_flag_symbol : public boost::spirit::symbols<>
{
	lv_flag_symbol()
	{
		add
			("NORMAL",Sally::CSally_change_chara::NORMAL)
			("AVERAGE",Sally::CSally_change_chara::AVERAGE)
			("MAX",Sally::CSally_change_chara::MAX)
		;
	}
};

} // namespace SLG end
} // namespace BMW end