/*
	katze 05/03/06
	BMWで使う武器ID
*/
#pragma once

namespace BMW{
namespace Weapon{

// 武器種別
namespace Kind{
enum eWeaponType{
	FIGHT,			// 格闘武器
	MAGIC,			// 魔術武器
	FIGHT_COLLAB,	// 合体格闘武器
	MAGIC_COLLAB,	// 合体魔術武器
	FIGHT_COND,		// 状態変化格闘武器
	MAGIC_COND,		// 状態変化魔術武器
	STATUS,			// 能力値変化武器
	CURE,			// 治癒武器
	REFILL,			// 補給武器
};
}

// 武器養成タイプ（成長・費用共通）
namespace Type{
enum eWeaponTrainingType{
	A,
	B,
	C,
	D,
	E,
	F,
};
}

namespace Field{

enum eFieldType{
	CENTER,	// 自分中心
	LINE,	// 直線
	THROW,	// 投げ込み
};

} // namespace Field end

namespace Special{

enum eSpecialType{
	NORMAL,					// 通常武器
	NORMAL_EXTEND,			// 通常武器で何か特殊効果あり妄想心音とか
	COUNTER_EXTEND,			// 反撃時に何かある
	COUNTER_SPECIAL,		// 反撃専用
};

}; // namespace Counter end

} // namespace Weapon end
} // namespace BMW end