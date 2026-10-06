/*
	katze 05/04/30
	思考ルーチンID
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Action{
enum eAction{
	PLAYER,
	NORMAL,
	WAIT,
	BATTERY,
	WALL_COUNTER,
	NORMAL_PARAM,
	FIELD_PARAM,
	WALL,
	AKABINE,
	NORMAL_EVAL,
	NOACT,
	FIELD_EVAL,
};

enum eMove{
	MOVE,
	MOVE_NO,
	MOVE_ATK,
	MOVE_BATTERY,	// 敵が射程の内側に入ってきてたら移動
	MOVE_ONLY,		// 必ず移動する
	MOVE_ONLY_ATK,  // 必ず移動して攻撃
};

enum eSnipe{
	CHANGE=-2,
	NO,
};

// 評価関数制御
enum eEvalMoveAtk{
	SHORT,
	LONG,
};

enum eEvalHP{
	LOW,
	HIGH,
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end