/*
	katze 06/11/23
	不撓不屈
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Futou : public IDataAbility
{/**
	不撓不屈
*/
public:
	// コンストラクタ
	CAbility_Futou(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data);
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

