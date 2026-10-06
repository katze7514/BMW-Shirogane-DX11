/*
	katze 11/03/21
	抽選箱クラス定義
*/
#pragma once

namespace katzeSDK{
namespace Math{

class CLottery
{/**
	抽選箱
	与えた確率で当たりが入っている
	くじが無くなったら補充される

	配列にくじをランダムに持って、何番目のくじをひくかを
	ランダムで選ぶ。引いたくじは配列から削除。
	配列のサイズが0になったら抽選箱を作り直す。
 */
public:
	CLottery(unsigned int nWinRate=0){ init(nWinRate); }

	// くじを生成
	//　@param[in] win_rate あたりの確率（100分率）
	void init(unsigned int nWinRate);

	// くじを引く
	// tureだったら当たり
	bool lot();

	// あたり確率
	unsigned int getWinRate()const{ return nWinRate_; }

private:
	// くじリスト
	// 1だったらあたり、0だったら外れ
	vector<unsigned int> vecLot_;

	// あたりの確率
	unsigned int nWinRate_;
};

} // namespace Math end
} // namespace katze end
