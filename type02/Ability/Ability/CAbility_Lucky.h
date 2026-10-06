/*
	katze 06/03/14
	LUCKY
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Lucky : public IDataAbility
{/**
	強運
*/
public:
	// コンストラクタ
	CAbility_Lucky(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

