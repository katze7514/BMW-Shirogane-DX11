/*
	katze 08/05/26
	PROVOコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{

class CSpirit_Provo : public Ability::IDataAbility
{/**
	加速
*/
public:
	// コンストラクタ
	CSpirit_Provo(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 相手に適用
	void applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	bool enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);
};

} // namespace Spirit end
} // namespace BMW end

