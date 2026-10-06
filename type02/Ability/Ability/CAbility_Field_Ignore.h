/*
	katze 06/06/27
	フィールド武器無効
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Field_Ignore : public IDataAbility
{/**
	フィールド武器無効キャラ
*/
public:
	// コンストラクタ
	CAbility_Field_Ignore(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

