/*
	katze 06/11/23
	DEATH_EX
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Death_Ex : public IDataAbility
{/**
	直視の魔眼・改

	補正値の他に気力140以上で、バリア無効化
*/
public:
	// コンストラクタ
	CAbility_Death_Ex(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	// EN消費
	int		getEN(int nAttr)const{ return 5; }
	
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

