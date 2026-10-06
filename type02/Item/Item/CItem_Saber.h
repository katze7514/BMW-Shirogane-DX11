/*
	katze 06/06/12
	アイテムSABER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Saber : public Ability::IDataAbility
{/**
	セイバーカード
	毎ターンHP+10%
*/
public:
	// コンストラクタ
	CItem_Saber(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Item end
} // namespace BMW end

