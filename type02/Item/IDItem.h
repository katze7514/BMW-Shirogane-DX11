/*
	katze 05/06/01
	update 06/06/12
	アイテム
*/
#pragma once

namespace BMW{
namespace Item{

enum eItem{
	NO=-1,		// 何もない
	MEDI,		// カリン豆
	CURRY,		// カレー
	SHIRO,		// 士郎の手料理
	MAGAZINE,	// マガジン
	MABO,		// マーボー
	OTSU,		// 乙
	WOTSU,		// 乙乙
	NEKO,		// 猫耳
	HOUI,		// 法衣
	BOOT,		// ブーツ
	KYUDO,		// 弓道着
	MAID,		// メイド服
	MEGANE,		// 眼鏡
	ROD,		// ファンタズムーンロッド
	GEM,		// 宝石
	EARRING,	// イヤリング
	RIBBON,		// リボン
	TIGER,		// タイガーストラップ
	SABER,		// セイバー
	LANCER,		// ランサー
	ARCHER,		// アーチャー
	RIDER,		// ライダー
	CASTER,		// キャスター
	ASSASSIN,	// アサシン：欠番
	BERSERKER,	// バーサーカー
	TRUE_ASSASSIN,// 真アサシン
	GIRU,		// ギル
	AVENGER,	// アヴェンジャー
	// ADVでのBP追加のために使う
	BP,
	// アンバー撃破特典
	AGONIST,	// A異常症
};

namespace Range{
enum eRange{
	STATUS,	// ステ影響	applyStatus
	WEAPON,	// 武器影響 applyWeapon
	USE,	// 回復系(消費系) use
};
} // namespace Range end

} // namespace Item end
} // namespace BMW end