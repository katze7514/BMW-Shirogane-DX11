/*
	katze 05/03/01
	精神コマンドID
*/
#pragma once

namespace BMW{
namespace Spirit{

// 精神コマンド
enum eSpiritCmd{
	NO=-1,		// 何も無し
	FIREBALL,	// 熱血
	SPIRIT,		// 魂
	AVOID,		// ひらめき
	TOUGH,		// 不屈
	DEFENCE,	// 鉄壁
	CONCENT,	// 集中
	HIT,		// 必中
	SYNC,		// 感応
	ACC,		// 加速
	JUMP,		// 跳躍
	AWAKE,		// 覚醒
	AGAIN,		// 再動
	GUTS,		// 根性
	VERYGUTS,	// ド根性
	TRUST,		// 信頼
	FRIEND,		// 友情
	SUPPLY,		// 補給
	HOPE,		// 期待
	POWER,		// 気合
	ENCOURAGE,	// 激励
	SNIPE,		// 狙撃
	DIRECT,		// 直撃
	CHARGE,		// 突撃
	SPY,		// 偵察
	WEAK,		// 脱力
	EASYON,		// てかげん
	MIRACLE,	// 奇跡
	FORTUNE,	// 幸運
	BLESS,		// 祝福
	EFFORT,		// 努力
	CHEER,		// 応援
	FAITH,		// 信念
	PRAY,		// 祈り

	// 第四部より追加
	PROVO,		// 挑発

	SPIRIT_END,
};

// 効果範囲
namespace Range{
enum eKind{
	SELF,	// 自分自身
	FRIEND,	// 味方
	ENEMY,	// 敵
};
} // namespace Range end

} // namespace Spirit end
} // namespace BMW end