/*
	katze 06/06/12
	アイテムSHIRO
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Shiro : public Ability::IDataAbility
{/**
	士郎の手料理
	SP+50
*/
public:
	// コンストラクタ
	CItem_Shiro(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Item end
} // namespace BMW end

