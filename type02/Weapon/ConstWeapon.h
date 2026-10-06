/*
	katze 05/03/13
	武器関係の定数やテーブル
*/
#pragma once

namespace BMW{
namespace Weapon{

class Const
{
public:
	// 武器ID変換
	static katzeSDK::Misc::CStringMap weaponID_;
	// 武器養成コスト
	const static int WEAPON_TRAINING_COST[5][11];
	// 武器養成値
	const static int WEAPON_TRAINING_VALUE[6][11];
};

} // namespace Weapon end
} // namespace BMW end