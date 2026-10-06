/*
	katze 05/05/10
	update 06/03/26
	シンボルシンボル
*/
#pragma once

#include "../../Demo/Code/CCode_gage.h"
#include "../../Demo/Code/CCode_back_visible.h"
#include "../../Demo/Back/IDemoBackLine.h"

#include "../IDMovieCode.h"
#include "CDataSymbolGui.h"

namespace BMW{
namespace Movie{

struct graphic_type_symbol : public boost::spirit::symbols<int>
{
	graphic_type_symbol()
	{
		add
			("NORMAL", CDataSymbolGraphic::NORMAL)
			("SIZE", CDataSymbolGraphic::SIZE)
			("ROTATE", CDataSymbolGraphic::ROTATE)
			("ROTATE2", CDataSymbolGraphic::ROTATE2)
			("ROTATE3", CDataSymbolGraphic::ROTATE3)
			("MORPH", CDataSymbolGraphic::MORPH)
			("AFFINE", CDataSymbolGraphic::AFFINE)
		;
	}
};

struct button_type_symbol : public boost::spirit::symbols<int>
{
	button_type_symbol()
	{
		add
			("NONE", CDataSymbolButton::NONE)
			("SPRITE", CDataSymbolButton::SPRITE)
			("SYMBOL", CDataSymbolButton::SYMBOL)
		;
	}
};

struct num_type_symbol : public boost::spirit::symbols<int>
{
	num_type_symbol()
	{
		add
			("SPRITE", CDataSymbolNum::SPRITE)
			("SYMBOL", CDataSymbolNum::SYMBOL)
		;
	}
};

struct movie_type_symbol : public boost::spirit::symbols<int>
{
	movie_type_symbol()
	{
		add
			("NORMAL", Clip::NORMAL)
			("DEMO", Clip::DEMO)
			("BORN", Clip::BORN)
		;
	}
};

struct movie_code_symbol : public boost::spirit::symbols<int>
{
	movie_code_symbol()
	{
		add
			("NO",	Code::NO)
			("SE",	Code::SE)
			("BACK",Code::BACK)
			("MES",	Code::MES)
		;
	}
};

struct back_vel_symbol : public boost::spirit::symbols<int>
{
	back_vel_symbol()
	{
		add
			("RIGHT_10",Demo::IDemoBackLine::RIGHT_10)
			("RIGHT_9",	Demo::IDemoBackLine::RIGHT_9)
			("RIGHT_8",	Demo::IDemoBackLine::RIGHT_8)
			("RIGHT_7",	Demo::IDemoBackLine::RIGHT_7)
			("RIGHT_6",	Demo::IDemoBackLine::RIGHT_6)
			("RIGHT_5",	Demo::IDemoBackLine::RIGHT_5)
			("RIGHT_4",	Demo::IDemoBackLine::RIGHT_4)
			("RIGHT_3",	Demo::IDemoBackLine::RIGHT_3)
			("RIGHT_2",	Demo::IDemoBackLine::RIGHT_2)
			("RIGHT_1",	Demo::IDemoBackLine::RIGHT_1)
			("STOP",	Demo::IDemoBackLine::STOP)
			("LEFT_1",	Demo::IDemoBackLine::LEFT_1)
			("LEFT_2",	Demo::IDemoBackLine::LEFT_2)
			("LEFT_3",	Demo::IDemoBackLine::LEFT_3)
			("LEFT_4",	Demo::IDemoBackLine::LEFT_4)
			("LEFT_5",	Demo::IDemoBackLine::LEFT_5)
			("LEFT_6",	Demo::IDemoBackLine::LEFT_6)
			("LEFT_7",	Demo::IDemoBackLine::LEFT_7)
			("LEFT_8",	Demo::IDemoBackLine::LEFT_8)
			("LEFT_9",	Demo::IDemoBackLine::LEFT_9)
			("LEFT_10",	Demo::IDemoBackLine::LEFT_10)
		;
	}
};

struct back_visible_symbol : public boost::spirit::symbols<int>
{
	back_visible_symbol()
	{
		add
			("true",1)
			("false",0)
		;
	}
};

struct back_visible_type_symbol : public boost::spirit::symbols<int>
{
	back_visible_type_symbol()
	{
		add
			("ALL",Demo::Code::CCode_back_visible::ALL)
			("BACK",Demo::Code::CCode_back_visible::BACK)
			("FORWARD",Demo::Code::CCode_back_visible::FORWARD)
		;
	}
};

struct gage_symbol : public boost::spirit::symbols<>
{
	gage_symbol()
	{
		using Demo::Code::CCode_gage;
		add
			("ATTACK_CHANGE",				CCode_gage::ATTACK_CHANGE)
			("ATTACK_HP",					CCode_gage::ATTACK_HP)
			("ATTACK_WEAPON_EN",			CCode_gage::ATTACK_WEAPON_EN)
			("ATTACK_SKILL_EN",				CCode_gage::ATTACK_SKILL_EN)
			("ATTACK_SKILL_DEF_EN",			CCode_gage::ATTACK_SKILL_DEF_EN)
			("COUNTER_CHANGE",				CCode_gage::COUNTER_CHANGE)
			("COUNTER_HP",					CCode_gage::COUNTER_HP)
			("COUNTER_WEAPON_EN",			CCode_gage::COUNTER_WEAPON_EN)
			("COUNTER_SKILL_EN",			CCode_gage::COUNTER_SKILL_EN)
			("COUNTER_SKILL_DEF_EN",		CCode_gage::COUNTER_SKILL_DEF_EN)
			("ATTACK_BACK_CHANGE",			CCode_gage::ATTACK_BACK_CHANGE)
			("ATTACK_BACK_HP",				CCode_gage::ATTACK_BACK_HP)
			("ATTACK_BACK_WEAPON_EN",		CCode_gage::ATTACK_BACK_WEAPON_EN)
			("ATTACK_BACK_SKILL_EN",		CCode_gage::ATTACK_BACK_SKILL_EN)
			("ATTACK_BACK_SKILL_DEF_EN",	CCode_gage::ATTACK_BACK_SKILL_DEF_EN)
			("COUNTER_BACK_CHANGE",			CCode_gage::COUNTER_BACK_CHANGE)
			("COUNTER_BACK_SKILL_DEF_EN",	CCode_gage::COUNTER_BACK_SKILL_DEF_EN)
			("INTRO",						CCode_gage::INTRO)
			("EXIT",						CCode_gage::EXIT)
		;
	};
};

} // namespace Movie end
} // namespace BMW end