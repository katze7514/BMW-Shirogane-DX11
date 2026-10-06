/*
	katze 06/03/14
	BREAKLINE
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Breakline : public IDataAbility
{/**
	見切り
*/
public:
	// コンストラクタ
	CAbility_Breakline(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int	getGetFP(int nAttr=0)const{ return 85; }
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

