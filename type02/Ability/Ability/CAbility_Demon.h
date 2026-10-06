/*
	katze 06/03/14
	DEMON
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Demon : public IDataAbility
{/**
	狂化
*/
public:
	// コンストラクタ
	CAbility_Demon(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// SLGデータ補正値計算
	// 個別対応
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

