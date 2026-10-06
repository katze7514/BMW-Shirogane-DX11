/*
	katze 06/06/12
	アイテムBOOT
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Boot : public Ability::IDataAbility
{/**
	ブーツ
	移動力+1、到達+2
*/
public:
	// コンストラクタ
	CItem_Boot(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 適用
	// ステータス適用
	void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void	applyStatus(Chara::CDataCharaInter& inter, int nAttr);
	void	backStatus(Chara::CDataCharaInter& inter, int nAttr);
};

} // namespace Item end
} // namespace BMW end

