/*
	katze 06/03/14
	MAGICIAN
*/
#pragma once

#include "../CMagicianTable.h"
#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Magician : public IDataAbility
{/**
	魔術師
*/
public:
	// コンストラクタ
	CAbility_Magician(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetRange& data, int nAttr);
	void applyOffset(SLG::COffsetHit& data, int nAttr);
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }

private:
	CMagicianTable	magicianTable_;
};

} // namespace Ability end
} // namespace BMW end

