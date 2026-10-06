/*
	katze 06/11/23
	ROAIAS
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Roaias : public IDataAbility
{/**
	熾天覆う七つの円冠
*/
public:
	// コンストラクタ
	CAbility_Roaias(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を使用するのに消費するEN
	int	getEN(int nAttr=0)const{ return 10; }
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

