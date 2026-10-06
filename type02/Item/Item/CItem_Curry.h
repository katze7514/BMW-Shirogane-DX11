/*
	katze 06/06/12
	アイテムCURRY
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Curry : public Ability::IDataAbility
{/**
	カレー
	EN全回復
*/
public:
	// コンストラクタ
	CItem_Curry(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

