/*
	katze 06/07/07
	ちびキャラ
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Loli : public IDataAbility
{/**
	ちびキャラ
*/
public:
	// コンストラクタ
	CAbility_Loli(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

