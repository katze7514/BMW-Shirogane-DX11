/*
	katze 06/03/14
	SCAEPGOAT
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Scaepgoat : public IDataAbility
{/**
	身代わり
*/
public:
	// コンストラクタ
	CAbility_Scaepgoat(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を使用するのに必要なEN
	int	getEN(int nAttr=0){ return 25; }
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

