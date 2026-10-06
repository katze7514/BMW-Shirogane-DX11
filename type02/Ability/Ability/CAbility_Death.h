/*
	katze 06/03/14
	DEATH
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Death : public IDataAbility
{/**
	直視の魔眼
*/
public:
	// コンストラクタ
	CAbility_Death(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data);
	void applyOffset(SLG::COffsetBattle& data);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

