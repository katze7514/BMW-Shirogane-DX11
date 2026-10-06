/*
	katze 06/03/14
	COUNTER
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Counter : public IDataAbility
{/**
	カウンター
*/
public:
	// コンストラクタ
	CAbility_Counter(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int	getGetFP(int nAttr=0)const;
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

