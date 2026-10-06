/*
	katze 06/03/14
	VAMPIRE
*/
#pragma once

#include "../CVampireTable.h"
#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Vampire : public IDataAbility
{/**
	吸血種
*/
public:
	// コンストラクタ
	CAbility_Vampire(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data, int nAttr);
	void applyOffset(SLG::COffsetBattle& data, int nAttr, SLG::CDataCharaSLG& slg);
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }

private:
	CVampireTable	vampireTable_;
};

} // namespace Ability end
} // namespace BMW end

