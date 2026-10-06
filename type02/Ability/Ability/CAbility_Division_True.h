/*
	katze 08/06/14
	TRUE_DIVISION
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Division_True : public IDataAbility
{/**
	真・分割思考
*/
public:
	// コンストラクタ
	CAbility_Division_True(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// ステータス適用
	void applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// SLGデータ補正値計算
	void applyOffset(SLG::COffsetHit& data, const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target);
};

} // namespace Ability end
} // namespace BMW end

