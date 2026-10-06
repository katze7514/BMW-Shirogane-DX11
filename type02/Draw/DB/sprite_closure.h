/*
	katze 05/03/24
	解析時に使うClosure
*/
#pragma once

#include "../CSpriteInfoBase.h"

namespace BMW{
namespace Draw{
// using宣言
using namespace boost::spirit;

// offsetルールクロージャ
struct point_closure : public closure<point_closure, POINT>
{
	member1 val;
};

// rectルールクロージャ
struct rect_closure : public closure<rect_closure, RECT>
{
	member1 val;
};

// スプライトルールクロージャ
struct sprite_closure : public closure<sprite_closure, CSpriteInfoBase, string>
{
	member1 val;
	member2 name;
};

} // namespace Draw end
} // namespace BMW end