/*
	katze 05/06/01
	精神constテーブル
*/
#pragma once

namespace BMW{
namespace Spirit{

class CSpiritName : public katzeSDK::Misc::CIntMap
{
public:
	// コンストラクタ
	CSpiritName();
};

class Const
{
public:
	// IDと名前のマップ
	static CSpiritName spiritName_;
};

} // namespace Spirit end
} // namespace BMW end