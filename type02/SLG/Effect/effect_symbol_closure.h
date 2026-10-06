/*
	katze 06/05/24
	エフェクトのシンボルとクロージャ
*/
#pragma once

#include "../../Scene/Unit/IDHelp.h"
#include "../Code/CCode_map_scroll.h"
#include "Code/CCode_load_symbol.h"
#include "Code/CCode_add_symbol.h"
#include "Code/CCode_ctrl_symbol.h"
#include "Code/CCode_skip.h"
#include "Code/CCode_ctrl_map_chip.h"

#include "CCmdSlgEffect.h"

namespace BMW{
namespace SLG{
namespace Effect{

struct load_symbol_closure : public boost::spirit::closure<load_symbol_closure, CCmdLoad>
{
	member1 val;
};

struct add_symbol_closure : public boost::spirit::closure<add_symbol_closure, CCmdAddSymbol, int>
{
	member1 val;
	member2 target;
};

struct ctrl_symbol_closure : public boost::spirit::closure<ctrl_symbol_closure, CCmdCtrl>
{
	member1 val;
};

struct map_scroll_closure : public boost::spirit::closure<map_scroll_closure, CCmdMapScroll, int>
{
	member1 val;
	member2 type;
};

struct map_chip_closure : public boost::spirit::closure<map_chip_closure, CCmdMapChip>
{
	member1 val;
};

struct load_effect_symbol : public boost::spirit::symbols<>
{
	load_effect_symbol()
	{
		add
			("NORMAL",CCode_load_symbol::NORMAL)
			("COMMON",CCode_load_symbol::COMMON)
		;

	}
};

struct add_event_target_symbol : public boost::spirit::symbols<>
{
	add_event_target_symbol()
	{
		add
			("EVENT",CCode_add_event_symbol::EVENT)
			("MAP",CCode_add_event_symbol::EVENT_MAP)
			("CHARA",CCode_add_event_symbol::EVENT_CHARA)
		;

	}
};

struct add_map_target_symbol : public boost::spirit::symbols<>
{
	add_map_target_symbol()
	{
		add
			("MAP",CCode_add_map_symbol::MAP)
			("CHARA",CCode_add_map_symbol::MAP_CHARA)
		;
	}
};

struct ctrl_symbol_symbol : public boost::spirit::symbols<>
{
	ctrl_symbol_symbol()
	{
		add
			("VISIBLE",CCode_ctrl_symbol::VISIBLE)
			("VALID",CCode_ctrl_symbol::VALID)
		;
	}
};

struct map_scroll_symbol : public boost::spirit::symbols<>
{
	map_scroll_symbol()
	{
		add
			("MAP",Code::CCode_map_scroll::MAP)
			("CHARA",Code::CCode_map_scroll::CHARA)
			("TWEEN",Code::CCode_map_scroll::TWEEN)
		;
	}
};

struct skip_flag_symbol : public boost::spirit::symbols<>
{
	skip_flag_symbol()
	{
		add
			("reset",CCode_skip::RESET)
			("false",CCode_skip::FALSE_SKIP)
			("true",CCode_skip::TRUE_SKIP)
		;
	}
};

struct help_symbol : public boost::spirit::symbols<>
{
	help_symbol()
	{
		add
			("SLG_START",			Unit::Help::SLG_START)
			("SLG_START2",			Unit::Help::SLG_START2)
			("SLG_CHARA_METOR",		Unit::Help::SLG_CHARA_METOR)
		;
	}
};

struct mapchip_type_symbol : public boost::spirit::symbols<>
{
	mapchip_type_symbol()
	{
		add
			("CHARA",	CCode_ctrl_map_chip::CHARA)
			("MAP",		CCode_ctrl_map_chip::MAP)
		;
	}
};

struct mapchip_ctrl_symbol : public boost::spirit::symbols<>
{
	mapchip_ctrl_symbol()
	{
		add
			("VISIBLE",	CCode_ctrl_map_chip::VISIBLE)
			("ADD",		CCode_ctrl_map_chip::ADD)
			("SWAP",	CCode_ctrl_map_chip::SWAP)
		;
	}
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end