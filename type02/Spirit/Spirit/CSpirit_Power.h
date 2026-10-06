/*
	katze 06/03/14
	POWERコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{

class CSpirit_Power : public Ability::IDataAbility
{/**
	気力
*/
public:
	// コンストラクタ
	CSpirit_Power(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void			use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);
};

} // namespace Spirit end
} // namespace BMW end

