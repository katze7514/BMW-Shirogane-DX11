/*
	katze 05/03/13
	キャラ関係の定数とかテーブルとか
*/
#pragma once

namespace BMW{
namespace Chara{

__inline int calcPer(const int nSource, const int nPer,const int nLv)
{
	return nLv*nSource*nPer/100;
}

class Const
{
public:
	// キャラIDのIDと文字列表現の相互変換
	static katzeSDK::Misc::CStringMap charaID_;
	// 性格IDと文字列表現のマップ
	static const string natureID2String[4];
	// HP養成%
	static const int HP_TRAINING_VALUE;
	// HP養成必要BP
	static const int HP_TRAINING_BP[11];
	// EN養成%
	static const int EN_TRAINING_VALUE;
	// EN養成必要BP
	static const int EN_TRAINING_BP[11];
	// 敏捷養成%
	static const int QUICK_TRAINING_VALUE;
	// 敏捷養成必要BP
	static const int QUICK_TRAINING_BP[11];
	// 耐久養成%
	static const int TOUGH_TRAINING_VALUE;
	// 耐久必要BP
	static const int TOUGH_TRAINING_BP[11];
	// 気力増減テーブル
	static const int MENTAL_UPDOWN_TABLE[4][8];
	// 基礎能力UPに必要なFP
	static const int FUND_TRAINING_FP;
};

} // namespace Chara end
} // namespace BMW end