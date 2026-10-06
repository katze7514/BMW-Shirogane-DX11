/*
	katze 06/03/14
	HITAWAY
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Hitaway : public IDataAbility
{/**
	ヒット＆アウェイ
*/
public:
	// コンストラクタ
	CAbility_Hitaway(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int				getGetFP(int nAttr=0)const{ return 70; }
	
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

