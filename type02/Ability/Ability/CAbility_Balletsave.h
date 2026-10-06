/*
	katze 06/06/27
	BALLETSAVE
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Balletsave : public IDataAbility
{/**
	弾数増強
*/
public:
	// コンストラクタ
	CAbility_Balletsave(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int	getGetFP(int nAttr=0)const{ return 80; }
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

