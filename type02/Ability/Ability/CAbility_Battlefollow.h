/*
	katze 06/03/14
	BATTLEFOLLOW
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Battlefollow : public IDataAbility
{/**
	戦闘続行
*/
public:
	// コンストラクタ
	CAbility_Battlefollow(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

