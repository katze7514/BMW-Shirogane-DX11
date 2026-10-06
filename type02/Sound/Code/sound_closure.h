/*
	katze 05/05/21
	サウンドクロージャ
*/
#pragma once

namespace BMW{
namespace Sound{
namespace Code{

struct sound_closure : public boost::spirit::closure<sound_closure, CCmdSound>
{
	member1 val;
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end