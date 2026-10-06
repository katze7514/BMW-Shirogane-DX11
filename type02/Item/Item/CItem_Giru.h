/*
	katze 06/06/12
	アイテムGIRU
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Giru : public Ability::IDataAbility
{/**
	ギルカード
	毎ターン弾数回復
*/
public:
	// コンストラクタ
	CItem_Giru(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用
	void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

