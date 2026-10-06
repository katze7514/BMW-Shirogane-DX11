/*
	katze 06/06/12
	アイテムTIGER
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Item{

class CItem_Tiger : public Ability::IDataAbility
{/**
	タイガーストラップ
	状態変化無効化
*/
public:
	// コンストラクタ
	CItem_Tiger(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext){ return true; }
};

} // namespace Item end
} // namespace BMW end

