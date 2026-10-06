/*
	katze 05/07/16
	回復アイテム
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Medi : public Ability::IDataAbility
{/**
	回復アイテム
	ようはリペアキット
*/
public:
	// コンストラクタ
	CItem_Medi(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	// 適用
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

