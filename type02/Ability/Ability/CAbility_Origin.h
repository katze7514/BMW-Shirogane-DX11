/*
	katze 06/03/14
	ORIGIN
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Origin : public IDataAbility
{/**
	真祖
*/
public:
	// コンストラクタ
	CAbility_Origin(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data);
	void applyOffset(SLG::COffsetBattle& data);
};

} // namespace Ability end
} // namespace BMW end

