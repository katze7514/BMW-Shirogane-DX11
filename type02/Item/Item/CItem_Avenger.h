/*
	katze 08/06/06
	アイテムAVENGER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Avenger : public Ability::IDataAbility
{/**
	アヴェンジャーカード
	30%の確率でカウンター
*/
public:
	// コンストラクタ
	CItem_Avenger(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

