/*
	katze 11/03/21
	抽選箱クラス実装
*/
#include "stdafx.h"

#include "CLottery.h"
#include <random>

namespace katzeSDK{
namespace Math{

// くじを生成
void CLottery::init(unsigned int nWinRate)
{
	vecLot_.clear();
	nWinRate_ = nWinRate;

	if(nWinRate_<=0)	return;
	if(nWinRate_>=100)	return;

	// あたりくじを入れる
	for(unsigned int i=0; i<nWinRate_; ++i)
		vecLot_.push_back(1);

	// はずれくじを入れる
	for(unsigned int i=0; i<100-nWinRate; ++i)
		vecLot_.push_back(0);

	// 配列をランダマイズ
	{ std::mt19937 rng(static_cast<unsigned int>(::GetTickCount())); std::shuffle(vecLot_.begin(), vecLot_.end(), rng); }
}

// くじを引く
// tureだったら当たり
bool CLottery::lot()
{
	if(nWinRate_<=0)	return false;
	if(nWinRate_>=100)	return true;

	// くじが空だったら作り直す
	if(vecLot_.empty()) init(nWinRate_);

	unsigned int index = ::rand() % vecLot_.size();
	unsigned int r = vecLot_[index];
	
	vector<unsigned int>::iterator it = vecLot_.begin();
	for(unsigned int i=0; i<index; ++i)
		++it;

	vecLot_.erase(it); // 引いたくじは削除

	return r;
}

} // namespace katzeSDK
} // namespace Math
