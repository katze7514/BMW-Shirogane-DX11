/*
	katze 06/03/14
	MADRED
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Madred : public IDataAbility
{/**
	紅赤朱
*/
public:
	// コンストラクタ
	CAbility_Madred(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	int getEN(int nAttr=0)const{ return 5; }
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

