/*
	katze 05/02/27
	解析時に使うClosure
*/
#pragma once

namespace BMW{
namespace Parser{
// using宣言
using namespace boost::spirit;

// nameなどで使うclosuer
struct string_closure : public closure<string_closure, string>
{
	member1 val;
};

// rectの要素やoffsetの要素で使うclosure
struct int_closure : public closure<int_closure,int>
{
	member1 val;
};

struct int_str_closure : public boost::spirit::closure<int_str_closure, int, string>
{
	member1 val;
	member2 str;
};

// stringクロージャ
struct ss_closure : public boost::spirit::closure<ss_closure, string, string>
{
	member1 val;
	member2 pre;
};

struct bool_closure : public closure<bool_closure, bool>
{
	member1 val;
};

} // namespace Closure end
} // namespace BMW end