/*
	katze 06/03/14
	FUNDPOWER
*/
#pragma once

#include "../CFundTable.h"
#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Fundpower : public IDataAbility
{/**
	底力
*/
public:
	// コンストラクタ
	CAbility_Fundpower(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int	 getGetFP(int nAttr=0)const;
	
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetHit& data, int nAttr,const SLG::CDataCharaSLG& base);
	void applyOffset(SLG::COffsetBattle& data, int nAttr, const SLG::CDataCharaSLG& base);

private:
	CFundTable	fundTable_;
};

} // namespace Ability end
} // namespace BMW end

