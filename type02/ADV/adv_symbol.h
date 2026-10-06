/*
	katze 05/05/21
	ADVシンボル
*/
#pragma once

#include "../Hero/IDHero.h"

#include "IDADV.h"
#include "CMsgBoard.h"
#include "Code/CCode_msg_state.h"
#include "Code/CCode_fade.h"
#include "Code/CCode_valid.h"
#include "Code/CCode_train.h"
#include "Code/CCode_train_all.h"
#include "Code/CItem_ctrl.h"

namespace BMW{
namespace ADV{

struct side_symbol : public boost::spirit::symbols<>
{
	side_symbol()
	{
		add
			("LEFT",CMsgBoard::LEFT)
			("RIGHT",CMsgBoard::RIGHT)
		;
	}
};

struct state_ctrl_symbol : public boost::spirit::symbols<>
{
	state_ctrl_symbol()
	{
		add
			("VALID",	Code::CCode_msg_state::VALID)
			("INVALID",	Code::CCode_msg_state::INVALID)
			("VISIBLE",	Code::CCode_msg_state::VISIBLE)
		;
	}
};

struct wait_symbol : public boost::spirit::symbols<>
{
	wait_symbol()
	{
		add
			("INPUT",Wait::INPUT)
			("BGM",Wait::BGM)
			("SE",Wait::SE)
			("FADE",Wait::FADE)
			// SLGでのみ使用
			//("SALLY",Wait::SALLY)
		;
	}
};

struct fade_ctrl_symbol : public boost::spirit::symbols<>
{
	fade_ctrl_symbol()
	{
		add
			("FADE_IN", Code::CCode_fade::FADE_IN)
			("FADE_OUT", Code::CCode_fade::FADE_OUT)
		;
	}
};

struct fade_color_symbol : public boost::spirit::symbols<>
{
	fade_color_symbol()
	{
		add
			("BLACK", Code::CCode_fade::BLACK)
			("WHITE", Code::CCode_fade::WHITE)
			("RED",   Code::CCode_fade::RED)
		;
	}
};

struct valid_symbol : public boost::spirit::symbols<>
{
	valid_symbol()
	{
		add
			("ADD", Code::CCode_valid::ADD)
			("DEL", Code::CCode_valid::DEL)
			("CLEAR",Code::CCode_valid::CLEAR)
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

struct train_kind_symbol : public boost::spirit::symbols<>
{
	train_kind_symbol()
	{
		add
			("EXP", Code::CCode_train::EXP)
			("KILL", Code::CCode_train::KILL)
			("CHANGE", Code::CCode_train::CHANGE)
			("COPY", Code::CCode_train::COPY)
			("BACK", Code::CCode_train::BACK)
			("CLEAR", Code::CCode_train::CLEAR)
			("DEL", Code::CCode_train::DEL)
		;
	}
};

struct train_type_symbol : public boost::spirit::symbols<>
{
	train_type_symbol()
	{
		add
			("ABS", Code::CCode_train::ABS)
			("ADD", Code::CCode_train::ADD)
		;
	}
};

struct train_ctrl_symbol : public boost::spirit::symbols<>
{
	train_ctrl_symbol()
	{
		add
			("NORMAL", Code::CCode_train_all::NORMAL)
			("SUB", Code::CCode_train_all::SUB)
		;
	}
};

struct item_ctrl_symbol : public boost::spirit::symbols<>
{
	item_ctrl_symbol()
	{
		add
			("ADD", API::CItem_ctrl::ADD)
			("DEL", API::CItem_ctrl::DEL)
		;
	}
};

struct scn_type_symbol : public boost::spirit::symbols<>
{
	scn_type_symbol()
	{
		add
			("NEXT", Scenario::NEXT)
			("MEKAHISUI", Scenario::MEKAHISUI)
		;
	}
};


} // namespace ADV end
} // namespace BMW end