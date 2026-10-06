/*
	katze 06/03/14
	FLOAT
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Float : public IDataAbility
{/**
	浮揚
*/
public:
	// コンストラクタ
	CAbility_Float(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

