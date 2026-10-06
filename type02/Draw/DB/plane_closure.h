/*
	katze 05/12/27
	解析時に使うClosure
*/
#pragma once

namespace BMW{
namespace Draw{
// using宣言
using namespace boost::spirit;

// スプライトルールクロージャ
struct s2_closure : public closure<s2_closure, pair<string, string> >
{
	member1 val;
};

} // namespace Draw end
} // namespace BMW end