/*
	katze 05/05/21
	サウンド系ID
*/
#pragma once

namespace BMW{
namespace Sound{

namespace Ctrl{
enum eCtrl{
	STOP,		// 停止
	PLAY,		// 再生
	PAUSE,		// ポーズ
	REPLAY,		// リプレイ
	FADE_OUT,	// フェードアウト
	FADE_IN,	// フェードイン
	PLAY_L,		// ループ再生
	PLAY_N,		// 強制再生
	PLAY_LN,		// 強制ループ再生
	WAIT,		// いろんな意味で待ち
};
} // namespace Ctrl end

} // namespace Sound end
} // namespace BMW end