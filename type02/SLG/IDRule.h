/*
	katze 05/04/17
	CRuleFactoryで管理されるRuleTaskのID
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Rule{
// 定義済みサブルーチンID
enum eRuleID{
	// 定義済みサブルーチン
	// つまり、API
	// エントリポイント
	MAIN,
	CONTINUE_START,
	// 基本動作
	MENU_SELECT,
	MENU_SELECT_PLAYER,
	MENU_SELECT_CPU,

	// キャラ系
	MENU_CHARA,
	// 移動系
	MOVE_RULE,
	MOVE_RULE_CPU,
	MOVE_RANGE,
	//MOVE_VIEW,
	MOVE_SELECT,
	MOVE_ROAD, //10
	MOVE_EXEC,
	// 攻撃系
	ATTACK_RULE,
	ATTACK_RULE_CPU,
	ATTACK_WEAPON,
	ATTACK_RANGE,
	//ATTACK_VIEW,
	ATTACK_SELECT,
	ATTACK_ACTION,
	ATTACK_DEMO,
	ATTACK_APPLY,
	ATTACK_DEL,//20
	ATTACK_RESULT,
	// フィールド武器攻撃系
	FIELD_RULE,
	FIELD_SELECT,
	FIELD_ATTACK,
	//MAP_EFFECT,
	// 精神系
	SPIRIT_RULE,
	SPIRIT_MENU,
	SPIRIT_SELECT,
	SPIRIT_APPLY,
	// アイテム系
	ITEM_RULE,
	ITEM_MENU,//30
	//ITEM_SELECT, SPIRIT_SELECTで代用
	ITEM_APPLY,
	// 治癒系
	CURE_RULE,
	//CURE_SELECT, ATTACK_SELECTで代用
	//CURE_APPLY, ATTACK_APPLYで代用
	// 補給系
	REFILL_RULE,
	//FUFILL_SELECT, CURE_SELECTで代用
	REFILL_APPLY,
	// ステータス
	STATUS_RULE,
	// 告白
	LOVE_RULE,
	// 待機
	WAIT_RULE,

	// SLG上でのデモ
	DEMO_MAP,

	// ターン系
	MENU_TURN,
	// フェーズ終了
	PHASE_INIT,//40
	// 勝利条件
	VICTORY_VIEW,
	VICTORY_CHECK,
	VICTORY_CHANGE,
	// 出撃キャラ
	SALLY_RULE,
	SALLY_VIEW,
	SALLY_SPIRIT,
	// 検索
	SEARCH_RULE,
	// システム
	SYSTEM_RULE,
	// セーブ
	SAVE_RULE,
	// 終了
	EXIT_RULE,//50

	// ステータス系
	MENU_STATUS,
	LAND_RULE,
	CHARA_RULE,

	// 出撃選択系
	SALLY_SELECT,
	ADD_CHARA,
	DEL_CHARA,
	CHANGE_CHARA,
	ADD_CHARA_MAP,
	DEL_CHARA_MAP,
	SETUP_WEAPON,//60
	//SET_PERS,

	// イベント系
	//MES_BOARD,
	WAIT_INPUT,
	WAIT_FADE,
	WAIT_SE,
	//WAIT_SALLY,
	EFFECT,
	WAIT_EFFECT,
	EVENT_BATTLE,
	WAIT_FRAME,
	MSG_BACK,

	// ペナルティ
	PENALTY_RULE,

	// セーブデータ反映
	DATA_REF,//70

	// マップ系
	MAP_CURSOL,	// カーソル誘導
	MAP_SCROLL,	// マップスクロール

	// イベントハンドラ
	SLG_START,		// SLGスタート時に呼ばれる
	SLG_END,		// SLG終了時に呼ばれる
	PHASE_START,	// フェーズ始めに呼ばれる
	PHASE_END,		// フェーズ終わりに呼ばれる
	CHARA_END,		// キャラの行動が終わる（待機する）と呼ばれる
	BATTLE_START,	// 戦闘始めに呼ばれる。BATTLE_ACTIONの直後
	BATTLE_END,		// 戦闘終わりに呼ばれる。BATTLE_APPLYの直後
	LOVE_START,		// 説得が選択されると呼ばれる
	// ユーザー定義サブルーチンはこれ以降のIDが振られる
	USER_DEF, // 81
};
} // namesspace Rule end
} // namespace SLG end
} // namespace BMW end