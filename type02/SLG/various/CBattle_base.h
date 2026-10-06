/*
	katze 05/07/20
	Battle系の基底クラス
*/
#pragma once

#include "CScript_base.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Battle{

class CBattle_base : public Script::CScript_base
{/**
	Battle系の基底クラス

	つっても、ようは判定のための便利メソッドを持ってるだけだが
 */
public:
	// デストラクタ
	virtual ~CBattle_base(){}

	// 判定
	static bool IsBattle(int nChara, CSLGContext& context);
	static bool IsBattle(const string& sChara, CSLGContext& context);
	static bool IsBattle(int nChara1, int nChara2, CSLGContext& context);
	static bool IsBattle(const string& sChara1, const string& sChara2, CSLGContext& context);

	// 残りHPがnRemain%以下だったらtrueを返す
	static bool IsHP(int nChara, int nRemain, CSLGContext& context);
	static bool IsHP(const string& sChara, int nRemain, CSLGContext& context);

	// 設定
	// 撤退予定だが、死んでたらDEATH_EVENT扱いにする
	static void setRemove(const string& sChara, CSLGContext& context);
};

} // namespace Battle end
} // namespace SLG end
} // namespace BMW end