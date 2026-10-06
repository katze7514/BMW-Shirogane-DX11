/*
	katze 06/03/14
	HP_RECOVER_L
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Hp_recover_l : public IDataAbility
{/**
	HP回復（大）
*/
public:
	// コンストラクタ
	CAbility_Hp_recover_l(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Ability end
} // namespace BMW end

