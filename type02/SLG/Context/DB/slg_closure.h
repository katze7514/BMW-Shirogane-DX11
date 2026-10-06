/*
	katze 05/06/29
	SLGクロージャー
*/
#pragma once

#include "SlgCmd.h"

namespace BMW{
namespace SLG{

struct action_closure : public boost::spirit::closure<action_closure, Code::CCmdAction>
{
	member1 val;
};

struct addChara_closure : public boost::spirit::closure<addChara_closure, Code::CCmdAddChara,string>
{
	member1 val;
	member2	s;
};

struct addCharaMap_closure : public boost::spirit::closure<addCharaMap_closure, Code::CCmdAddCharaMap>
{
	member1 val;
};

struct delCharaMap_closure : public boost::spirit::closure<delCharaMap_closure, Code::CCmdDelCharaMap>
{
	member1 val;
};

struct msg_closure : public boost::spirit::closure<msg_closure, Code::CCmdMsg, string>
{
	member1 val;
	member2 s;
};

struct s_i_closure : public boost::spirit::closure<s_i_closure, string, int>
{
	member1 val;
	member2 no;
};

} // namespace SLG end
} // namespace BMW end