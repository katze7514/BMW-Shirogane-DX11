/*
	katze 06/06/27
	状態変化無効
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_CondIgnore : public IDataAbility
{/**
	状態変化無効
*/
public:
	// コンストラクタ
	CAbility_CondIgnore(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

