/*
	katze 06/03/14
	PRAYコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{

class CSpirit_Pray : public Ability::IDataAbility
{/**
	祈り
*/
public:
	// コンストラクタ
	CSpirit_Pray(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void	use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);
	bool	enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
};

} // namespace Spirit end
} // namespace BMW end

