/*
	katze 06/03/14
	DEFENCEコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{

class CSpirit_Defence : public Ability::IDataAbility
{/**
	鉄壁
*/
public:
	// コンストラクタ
	CSpirit_Defence(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);
};

} // namespace Spirit end
} // namespace BMW end

