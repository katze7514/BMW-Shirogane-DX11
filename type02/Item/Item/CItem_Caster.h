/*
	katze 06/06/12
	アイテムCASTER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Caster : public Ability::IDataAbility
{/**
	キャスターカード
	毎ターンEN+10%回復
*/
public:
	// コンストラクタ
	CItem_Caster(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Item end
} // namespace BMW end

