/*
	katze 06/03/14
	TWELVECROSS
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Twelvecross : public IDataAbility
{/**
	十二の試練
*/
public:
	// コンストラクタ
	CAbility_Twelvecross(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

