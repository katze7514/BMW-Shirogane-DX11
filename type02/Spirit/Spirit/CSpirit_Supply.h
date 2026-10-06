/*
	katze 06/03/14
	SUPPLYコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{

class CSpirit_Supply : public Ability::IDataAbility
{/**
	補給
*/
public:
	// コンストラクタ
	CSpirit_Supply(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	// 補給だけ引数が違う
	void applyStatus(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	bool enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);
};

} // namespace Spirit end
} // namespace BMW end

