/*
	katze 08/06/23
	地形効果無効
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Land_Ignore : public IDataAbility
{/**
	フィールド武器無効キャラ
*/
public:
	// コンストラクタ
	CAbility_Land_Ignore(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

