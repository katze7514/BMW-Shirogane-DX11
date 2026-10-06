/*
	katze 05/05/10
	シンボルクロージャ
*/
#pragma once

namespace BMW{
namespace Movie{

// DrawInfoクロージャ
struct draw_closure : public boost::spirit::closure<draw_closure, Draw::CDrawInfo>
{
	member1 val;
};

} // namespace Movie end
} // namespace BMW end