/*
	katze 06/06/12
	アイテムBERSERKER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Berserker : public Ability::IDataAbility
{/**
	バーサーカーカード
	HP+1500
	Tough+250
*/
public:
	// コンストラクタ
	CItem_Berserker(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);
};

} // namespace Item end
} // namespace BMW end

