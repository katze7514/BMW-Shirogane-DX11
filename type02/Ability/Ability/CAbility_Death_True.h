/*
	katze 06/11/23
	TRUE_DEATH
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Death_True : public IDataAbility
{/**
	真・直視の魔眼

	補正値の他に気力130以上で、分身
*/
public:
	// コンストラクタ
	CAbility_Death_True(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	// EN消費
	int		getEN(int nAttr=0)const{ return 5; }
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data);
	void applyOffset(SLG::COffsetBattle& data);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
	bool enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

