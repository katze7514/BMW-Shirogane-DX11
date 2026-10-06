/*
	katze 06/03/14
	EN_RECOVER_L
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_En_recover_l : public IDataAbility
{/**
	EN回復（大）
*/
public:
	// コンストラクタ
	CAbility_En_recover_l(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int				getGetFP(int nAttr=0);
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Ability end
} // namespace BMW end

