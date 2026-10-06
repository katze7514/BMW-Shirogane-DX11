/*
	katze 06/06/12
	アイテムOTSU
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Otsu : public Ability::IDataAbility
{/**
	乙
	HP+500、Tough+100
*/
public:
	// コンストラクタ
	CItem_Otsu(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);
};

} // namespace Item end
} // namespace BMW end

