/*
	katze 08/06/06
	でかキャラ
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Hiyokurenri : public IDataAbility
{/**
	でかキャラ
*/
public:
	// コンストラクタ
	CAbility_Hiyokurenri(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

