/**
	katze 06/05/19
	update 08/06/27

	体験版仕様とか、そういうのを切り替えるためのヘッダ
	デバッグモードとかの真に全部関わるものでは無く
	ゲームとしての差異を作る
*/
#pragma once

namespace BMW{

// デバッグモード時にのみに有効なやつを
#ifdef BMW_DEBUG

//#define HP_DIV // 敵HPを100分の1にするデバッグモード
//#define BMW_CAPTURE // 画面キャプモード
//#define STAGE_CREATE // ステージに敵配置可能モード
//#define CHARA_CTRL // キャラ操作モード
//#define EXPERT_NORMAL // 強制NORMALモード

#endif // BMW_DEBUG

// コンテニューデータファイル名
const string sContinue ="data4.con";

} // namespace BMW end