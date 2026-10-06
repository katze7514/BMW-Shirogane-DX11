/*
	katze 06/03/14
	ARROWBLESS
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Arrowbless : public IDataAbility
{/**
	矢避けの加護
*/
public:
	// コンストラクタ
	CAbility_Arrowbless(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// SLGデータ補正値
	void	applyOffset(SLG::COffsetHit& data);
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

