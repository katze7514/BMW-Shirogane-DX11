/*
	katze 11/03/21
	抽選箱乱数
*/
#pragma once

#include "CLottery.h"

namespace katzeSDK{
namespace Math{

class CRandLottery
{/**
	抽選箱を使った乱数（100分率）
 */
public:
	// コンストラクタ
	CRandLottery(){ init(); }

	// 抽選箱生成
	void init();

	// 抽選
	// 100分率で入れる
	bool lot(unsigned int rate);

private:
	CLottery aLottery_[100];
};

} // namespace Math end
} // namespace katzeSDK end
