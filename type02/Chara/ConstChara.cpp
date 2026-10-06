#include "stdafx.h"

#include "ConstChara.h"

namespace BMW{
namespace Chara{

// キャラIDのIDと文字列表現の相互変換
katzeSDK::Misc::CStringMap Const::charaID_;
// 性格IDと文字列表現のマップ
const string Const::natureID2String[4]={"弱気","普通","強気","超強気"};
// HP養成%
const int Const::HP_TRAINING_VALUE=5;
// HP養成必要BP
const int Const::HP_TRAINING_BP[11]=
{ 0,200,400,600,800,1000,1200,1400,1600,1800,2000 };
// EN養成%
const int Const::EN_TRAINING_VALUE=10;
// EN養成必要BP
const int Const::EN_TRAINING_BP[11]=
{ 0,100,200,200,300,300,400,400,500,500,600 };
// 敏捷養成%
const int Const::QUICK_TRAINING_VALUE=5;
// 敏捷養成必要BP
const int Const::QUICK_TRAINING_BP[11]=
{ 0,500,800,1000,1300,1500,2000,2500,3000,3500,4000 };
// 耐久養成%
const int Const::TOUGH_TRAINING_VALUE=5;
// 耐久養成必要BP
const int Const::TOUGH_TRAINING_BP[11]=
{ 0,300,500,800,1000,1500,2000,2500,3000,3500,4000 };
// 気力増減テーブル
const int Const::MENTAL_UPDOWN_TABLE[4][8]=
{
	0,1,0,3,-1,4,1,-1,
	0,0,0,0,1,4,1,0,
	0,1,1,1,1,4,1,1,
	0,2,-1,0,2,4,1,2
};
// 基礎能力UPに必要なFP
const int Const::FUND_TRAINING_FP=6;

} // namespace Chara end
} // namespace BMW end