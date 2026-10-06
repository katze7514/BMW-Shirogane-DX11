/*
	katze 06/06/27
	でかキャラ
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Deka : public IDataAbility
{/**
	でかキャラ
*/
public:
	// コンストラクタ
	CAbility_Deka(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

