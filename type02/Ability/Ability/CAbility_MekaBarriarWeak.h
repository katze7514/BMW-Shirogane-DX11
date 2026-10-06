/*
	katze 09/02/02
	MEKA_BARRIER_WEAK
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_MekaBarriarWeak : public IDataAbility
{/**
	メカヒスイバリア（弱）
*/
public:
	// コンストラクタ
	CAbility_MekaBarriarWeak(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	int	getEN(int nAttr=0)const{ return 5; }
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

