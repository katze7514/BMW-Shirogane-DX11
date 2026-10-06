/*
	katze 05/05/21
	サウンドシンボル
*/
#pragma once

#include "../IDSound.h"

namespace BMW{
namespace Sound{
namespace Code{

struct sound_ctrl_symbol : public boost::spirit::symbols<>
{
	sound_ctrl_symbol()
	{
		add
			("STOP",		Ctrl::STOP)
			("PLAY",		Ctrl::PLAY)
			("PAUSE",		Ctrl::PAUSE)
			("REPLAY",		Ctrl::REPLAY)
			("FADE_OUT",	Ctrl::FADE_OUT)
			("FADE_IN",		Ctrl::FADE_IN)
			("PLAY_L",		Ctrl::PLAY_L)
			("PLAY_N",		Ctrl::PLAY_N)
			("PLAY_LN",		Ctrl::PLAY_LN)
			("WAIT",		Ctrl::WAIT)
		;
	};
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end