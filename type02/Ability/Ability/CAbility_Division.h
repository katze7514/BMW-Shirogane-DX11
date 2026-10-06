/*
	katze 06/03/14
	DIVISION
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Division : public IDataAbility
{/**
	分割思考
*/
public:
	// コンストラクタ
	CAbility_Division(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data, const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target);
};

} // namespace Ability end
} // namespace BMW end

