/*
	katze 06/06/12
	アイテムMABO
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Mabo : public Ability::IDataAbility
{/**
	マーボー
	HP・EN・弾数全回復
*/
public:
	// コンストラクタ
	CItem_Mabo(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

