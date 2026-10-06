/*
	katze 06/03/14
	EN_RECOVER_M
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_En_recover_m : public IDataAbility
{/**
	EN回復（中）
*/
public:
	// コンストラクタ
	CAbility_En_recover_m(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Ability end
} // namespace BMW end

