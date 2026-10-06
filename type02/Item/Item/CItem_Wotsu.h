/*
	katze 06/06/12
	アイテムWOTSU
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Wotsu : public Ability::IDataAbility
{/**
	乙乙
	HP+1000、Tough+150
*/
public:
	// コンストラクタ
	CItem_Wotsu(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);
};

} // namespace Item end
} // namespace BMW end

