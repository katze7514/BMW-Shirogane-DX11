/*
	katze 05/03/24
	SLGシーンで使うID
*/
#pragma once

namespace BMW{
namespace SLG{

namespace Act{
enum eAct
{
	BEFORE,		// 行動前
	MOVE,		// 移動後
	HIT_AWAY,		// 一撃離脱 or 二回攻撃
	HIT_AWAY_MOVE,	// 一撃離脱移動 or 二回攻撃 後
	AFTER,		// 行動後
	DEATH,		// 死亡
	DEATH_EVENT,// イベント的死亡扱い（ATTACK_DELでは削除されないが、RESULTではDEATHと同じ扱い）
	REMOVE,		// 離脱
};
} // namespace Act end

namespace Way{
enum eWay
{
	NO=-1,	
	TOP,	// 上 y-1
	LEFT,	// 左 x-1
	BOTTOM,	// 下 y+1
	RIGHT,	// 右 x+1
	ALL,	// フィールド武器用全方位
};
} // nameespce Way end

namespace Phase{
enum ePhase
{
	PLAYER,		// 味方フェイズ
	ENEMY,		// 敵フェイズ
	NEUTRAL,	// 中立フェイズ
};
} // namespace Phase

namespace Mental{
enum eMental
{
	NO,				// 何も無し
	HIT,			// 攻撃が命中
	MISS,			// 攻撃をはずした
	AVOID,			// 攻撃を避けた
	DAMAGE,			// 攻撃を受けた
	FALL,			// 撃墜した
	FALL_FIREND,	// 仲間が撃墜した
	FALL_ENEMY,		// 敵が撃墜した
};
} // namespace Mental end

namespace Battle{
// 戦闘時のアクションID
// 状態選択時もこれを使う
// 反撃するときは正の値になるしね
enum eAction{
	NO			= -4,	// 何もしない（反撃しない）
	HIT,				// 攻撃をくらった(反撃不能)
	DEFENCE,			// 防御
	AVOID,				// 回避
};
} // namespace Battle end

namespace Victory{
enum eVictory{
	NO=-1,
	VICTORY,
	LOSE,
	EXPERT,
#ifdef BMW_DEBUG
	RESTART,
#endif
};
} // namespace Victory end

namespace Expert{
enum eExpert{
	NORMAL,
	HARD,
	HEAVY,
	HELL,
	ALL_BEFORE,
	ALL_AFTER,
	// 熟練度45越え
	// エクストラステージ専用判定
	OVER_45,
};

enum eValid{
	INVISIBLE=-2,
	INVALID=-1,
};
} // namespace Expert end

namespace Flag{
// SLG VMの一時データに確保されるフラグ
// 若干、空きがあるのは下に追加していくなら、
// コンテニューデータは使えなくならないという意図
enum eFlag{
	CONTINUE		=-24,	// コンテニューから来たか(0)/来てないか(1)
	ID				,		// 現在のSLG ID
	TURN			,		// 現在のターン数
	BP				,		// 現在のBP
	FP				,		// 現在のFP
	EXPERT			,		// 現在の熟練度
	PHASE			,		// 現在のフェーズ
	PHASE_CHANGE	,		// フェーズが切り替わった直後に1になっている
	ENEMY_RESET		,		// エネミーイテレーションリセットフラグ
	VICTORY			,		// 現在有効な勝利条件No.
	LOSE			,		// 現在有効な敗北条件No.
	EXPERT_C		,		// 現在有効な熟練度条件No.
	DEMO			,		// 現在のDEMOのON(1)/OFF(0)
	NEXT_WEAPON_ID	,		// 次に追加する武器 SLG ID
	TARGET_MAP		,		// 現在の対象MapIndex
	TARGET_WEAPON	,		// 現在の対象武器SLG ID
	CTRL_WEAPON		,		// 現在の操作武器SLG ID
	TARGET_CHARA	,		// 現在の対象キャラSLG ID
	CTRL_CHARA		,		// 現在の操作キャラSLG ID
	TARGET_ABILITY	,		// 現在の対象の精神とかアイテムとか
	// 第三部より追加
	NEXT_SALLY_ID	,		// 次に追加するキャラ SLG ID
	WIPEOUT			,		// 全滅後やりなおした(1)/してない(0)
	//SALLY_INIT		,		// WAIT_SALLYに来ると初期化(1)される
	// 第四部より追加
	ADD_SLG_ID		,		// デバッグモード時だけ有効。次に追加するSLG ID
};

} // namespace Flag end


namespace MapChip{
enum eSprite
{// スプライトIndex
	GRID,
	ACTIVE,
	MOVE,
	ATTACK_CORE,
	ATTACK,
};

} // namespace MapChip end

namespace Pos{
enum ePos{
	CHIP,
	CHIP_TARGET,
	MOUSE,
	CHARA,
	CHARA_TARGET,
};
} // namespace Menu end

namespace ChipMovie{
// マップキャラチップアニメID
enum eCharaChipAnime{
	// マップ上
	// 歩き
	WALK_TOP,		// 上
	WALK_LEFT,		// 左
	WALK_BOTTOM,	// 下
	WALK_RIGHT,		// 右
	// ジャンプしゃがみ
	JUMP_READY_TOP,		// 上
	JUMP_READY_LEFT,	// 左
	JUMP_READY_BOTTOM,	// 下
	JUMP_READY_RIGHT,	// 右
	// ジャンプ上昇
	JUMP_UP_TOP,		// 上
	JUMP_UP_LEFT,		// 左
	JUMP_UP_BOTTOM,		// 下
	JUMP_UP_RIGHT,		// 右
	// ジャンプ下降
	JUMP_DOWN_TOP,		// 上
	JUMP_DOWN_LEFT,		// 左
	JUMP_DOWN_BOTTOM,	// 下
	JUMP_DOWN_RIGHT,	// 右
	// 静止
	BEFORE_TOP,		// 上
	BEFORE_LEFT,	// 左
	BEFORE_BOTTOM,	// 下
	BEFORE_RIGHT,	// 右
	// 行動済み静止
	AFTER_TOP,		// 上
	AFTER_LEFT,		// 左
	AFTER_BOTTOM,	// 下
	AFTER_RIGHT,	// 右
	// 静止ピンチ
	BEFORE_PINCH_TOP,		// 上
	BEFORE_PINCH_LEFT,		// 左
	BEFORE_PINCH_BOTTOM,	// 下
	BEFORE_PINCH_RIGHT,		// 右
	// 行動済み静止ピンチ
	AFTER_PINCH_TOP,		// 上
	AFTER_PINCH_LEFT,		// 左
	AFTER_PINCH_BOTTOM,		// 下
	AFTER_PINCH_RIGHT,		// 右
	// 戦闘デモOFFとかパネル上
	END, // 便宜的なEND列挙
};

} // namespace Movie end

} // namespace SLG end
} // namespace BMW end