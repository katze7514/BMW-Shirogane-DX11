/*
	katze 06/03/14
	SPECTER
*/
#pragma once

#include "../CSpecterTable.h"
#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Specter : public IDataAbility
{/**
	妖怪
*/
public:
	// コンストラクタ
	CAbility_Specter(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data, int nAttr);
	void applyOffset(SLG::COffsetBattle& data, int nAttr);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }

private:
	CSpecterTable specterTable_;
};

} // namespace Ability end
} // namespace BMW end

