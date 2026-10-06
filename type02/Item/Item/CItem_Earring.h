/*
	katze 06/06/12
	アイテムEARRING
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Earring : public Ability::IDataAbility
{/**
	イヤリング
	出撃時気力+10
*/
public:
	// コンストラクタ
	CItem_Earring(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);
};

} // namespace Item end
} // namespace BMW end

