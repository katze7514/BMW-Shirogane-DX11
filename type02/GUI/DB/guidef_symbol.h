/*
	katze 06/01/20
	GuiDef定義ファイル
*/
#pragma once

#include "../../Face/DB/CDataFace.h"
#include "IDGuiDef.h"

namespace BMW{
namespace GUI{

struct font_symbol : public boost::spirit::symbols<>
{
	font_symbol()
	{
		add
			("GOTHIC",CText::FONT_GOTHIC)
			("MINCHO",CText::FONT_MINCHO)
			("P_GOTHIC",CText::FONT_P_GOTHIC)
			("P_MINCHO",CText::FONT_P_MINCHO)
		;
	}
};

struct side_symbol : public boost::spirit::symbols<>
{
	side_symbol()
	{
		add
			("LEFT",Face::CDataFace::LEFT)
			("RIGHT",Face::CDataFace::RIGHT)
		;
	}
};

struct text_type_symbol : public boost::spirit::symbols<>
{
	text_type_symbol()
	{
		add
			("NORMAL",Text::NORMAL)
			("POPUP",Text::POPUP)
			("SIZE",Text::SIZE)
		;
	}
};

struct side_f_symbol : public boost::spirit::symbols<>
{
	side_f_symbol()
	{
		add
			("LEFT",Text::LEFT)
			("CENTER",Text::CENTER)
			("RIGHT",Text::RIGHT)
		;
	}
};

struct button_symbol : public boost::spirit::symbols<>
{
	button_symbol()
	{
		add
			("NORMAL",Button::NORMAL)
			("KEEP",Button::KEEP);
		;
	}
};

struct panel_symbol : public boost::spirit::symbols<>
{
	panel_symbol()
	{
		add
			("NORMAL",Panel::NORMAL)
			("CTRL",Panel::CTRL)
		;
	}
};

struct obj_symbol : public boost::spirit::symbols<>
{
	obj_symbol()
	{
		add
			("TASK",Obj::TASK)
			("BUTTON",Obj::BUTTON)
			("KEEP",Obj::KEEP)
			("GRAPHIC",Obj::GRAPHIC)
		;
	}
};


} // namespace GUI end
} // namespace BMW end