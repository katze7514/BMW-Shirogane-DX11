/*
	katze 05/03/06
	パーサデバッグ用Functor
*/
#pragma once

namespace BMW{
namespace Parser{

struct print_tag
{
	void operator()(const char* first, const char* last) const
	{
	#ifdef BMW_DEBUG
		string s(first,last);
		CDbg().Out(s);
	#endif
	}
};

struct print_int
{
	void operator()(int n) const
	{
	#ifdef BMW_DEBUG
		CDbg().Out(n);
	#endif
	}
};

struct print_string
{
	void operator()(const string& s) const
	{
	#ifdef BMW_DEBUG
		CDbg().Out(s);
	#endif
	}
};

} // namespace Parser end
} // namespace BMW end