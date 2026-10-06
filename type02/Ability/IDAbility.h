/*
	katze 05/03/02
	技能ID
*/
#pragma once

namespace BMW{
namespace Ability{
// 技能
enum eAbility{
	NO=-1,			// 何も無し

	// 技能
	SPECTER,		// 妖怪
	MAGICIAN,		// 魔術師
	VAMPIRE,		// 吸血種
	FUNDPOWER,		// 底力
	COUNTER,		// カウンター
	BACKUPATTACK,	// 援護攻撃
	BACKUPDEFENCE,	// 援護防御
	LINKAGEATTACK,	// 連携攻撃
	ATTACKER,		// アタッカー
	REVENGE,		// リベンジ
	GUARD,			// ガード
	BREAKLINE,		// 見切り
	HITAWAY,		// 一撃離脱
	MAGICSAVE,		// 魔力節約
	BATTLESPIRIT,	// 闘争心
	FIGHTUP,		// 戦意高揚
	RITHM,			// リズム感
	AGAINST,		// 対抗心
	SPUP,			// SPアップ
	SPRECOVER,		// SP回復
	CONCENT,		// 集中力
	LUCKY,			// 強運
	
	// 固有能力
	MAGICBARRIER_A,	// 対魔力障壁A
	MAGICBARRIER_B,	// 対魔力障壁B
	MAGICBARRIER_C,	// 対魔力障壁C
	MAGICRELEASE,	// 魔力放出
	AVALON,			// 全て遠き理想郷
	ARROWBLESS,		// 矢よけの加護
	BATTLEFOLLOW,	// 戦闘続行
	ROAIAS,			// 熾天覆う七つの円冠
	SUPERPOWER,		// 怪力
	DEMON,			// 狂化
	TWELVECROSS,	// 十二の試練
	DEATH,			// 直死の魔眼
	ORIGIN,			// 真祖
	MADRED,			// 紅赤朱
	SYNC,			// 感応
	DIVISION,		// 分割思考
	DEATH_TRUE,		// 真・直視の魔眼
	SCAPEGOAT,		// 身代わり
	FUTUREEYE,		// 未来視
	FLY,			// 飛行
	FUTSUNO,		// フツノバリア
	FLOAT,			// 浮揚
	HP_RECOVER_S,	// HP回復(小)
	HP_RECOVER_M,	// HP回復(中)
	HP_RECOVER_L,	// HP回復(大)
	EN_RECOVER_S,	// EN回復(小)
	EN_RECOVER_M,	// EN回復(中)
	EN_RECOVER_L,	// EN回復(大)

	// 第二部より追加
	// 技能
	MOVE_UP,		// 移動力UP
	BALLETSAVE,		// 弾数増強
	IKIYOYO,		// 意気揚々

	// 固有能力
	ALTER_EGO,		// 分身
	COND_IGNORE,	// 状態変化無効
	LOLI,			// ちびキャラ
	DEKA,			// でかキャラ

	// 第三部より追加
	// 固有能力
	DEATH_EX,		// 直視の魔眼・改
	FUTOU,			// 不撓不屈
	FIELD_IGNORE,	// フィールド武器無効
	KEHAI_SHADAN,	// 気配遮断

	// 第四部より追加
	// 固有能力
	CHALICE_CONECT,		// 聖杯連結
	MEKA_BARRIAR,		// メカヒスイバリア
	FUTUREEYE_SECOND,	// 未来視・弐
	TWICE_ACTION,		// 二回行動
	HIYOKURENRI,		// 比翼連理
	TRUE_DIVISION,		// 真・分割思考
	COLA_BARRIAR,		// コーラバリア
	OPEN_GET,			// オープンゲット
	MUDAI_SHIELD,		// ムダイシールド
	LAND_IGNORE,		// 地形効果無視

	// ベスト版より追加
	// 固有能力
	TWICE_ATTACK,		// 二回攻撃/一気呵成
	TWICE_ATTACK_ENEMY,	// 二回攻撃/一気呵成（敵）
	MEKA_BARRIAR_WEAK,	// メカヒスイバリア（弱）
};

} // namespace Abilty end
} // namespace BMW end