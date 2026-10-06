/*
	katze 08/06/03
	MUDAI_SHIELD
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_MudaiShield : public IDataAbility
{/**
	対魔力障壁A
*/
public:
	// コンストラクタ
	CAbility_MudaiShield(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
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

