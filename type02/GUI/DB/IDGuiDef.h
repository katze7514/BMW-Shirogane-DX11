/*
	katze 06/01/20
	GUI定義ID	
*/
#pragma once

namespace BMW{
namespace GUI{

namespace Def{
enum eDef{
	GAGE,
	PANEL,
	ACTION,
	CIRCLE,
	REMAIN,
	BUTTON,
	TEXT,
	GRAPHIC,
};
} // namespace Def end

namespace Widget{
enum eWidget{
	NAME,	// キャラ名
	FACE,	// 顔
	SYMBOL, // シンボル
	GRAPHIC,// グラフィック
	BUTTON, // ボタン
	TEXT,	// テキスト
	NUM,	// 数字
	REMAIN,	// リメイン数値
	GAGE,	// ゲージ
	PANEL,	// パネル
	OBJ,	// プレースホルダー
};
} // namespace Widget end

namespace Text{
enum eType{
	NAME=-1,
	NORMAL,
	POPUP,
	SIZE,
};
// 寄せ位置
enum eSide{
	LEFT,
	CENTER,
	RIGHT,
};
} // namepsace Text end

namespace Button{
enum eAct{
	NORMAL,
	KEEP,
};
} // namespace Button end
namespace Panel{
// パネル種別
enum eKind{
	NORMAL,
	CTRL,
};
} // namespace Panel end

namespace Obj{
// プレースホルダ種別
enum eType{
	TASK,
	BUTTON,
	KEEP,
	GRAPHIC,
};
} // namespace Obj end

} // namespace GUI end
} // namespace BMW end