/*
	katze 05/02/27
	解析用シンボルテーブル
*/
#pragma once

namespace BMW{
namespace Parser{
// using宣言
using namespace boost::spirit;

struct planesym : public symbols<int>
{
	void setSymbol(const string& sFile);
};

struct boolsym : public symbols<bool>
{
	boolsym()
	{
		add("true",true)("false",false);

	}
};

} // namespace Symbol end
} // namespace BMW end