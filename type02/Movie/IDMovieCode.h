/*
	katze 05/05/10
	Movie用コードID
*/
#pragma once

namespace BMW{
namespace Movie{

namespace Code{
enum eCode{
	NO,
	SE,
	SE_WAIT,
	STOP,
	STOP_MOVIE,
	END,
	// 以下は戦闘アニメ用
	BACK,
	BACK_VISIBLE,
	MES,
	GAGE,

	BGM,
};
} // namespace Code end

namespace Clip{
enum eClip{
	NORMAL,
	DEMO,
	BORN,
};
} // namespace Clip end

} // namespace Movie end
} // namespace BMW end