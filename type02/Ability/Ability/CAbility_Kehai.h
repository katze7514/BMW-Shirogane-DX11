/*
	katze 06/11/23
	気配遮断
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Kehai : public IDataAbility
{/**
	気配遮断
*/
public:
	// コンストラクタ
	CAbility_Kehai(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

