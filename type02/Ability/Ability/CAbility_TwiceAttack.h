/*
	katze 09/01/05
	二回攻撃/一気呵成
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_TwiceAttack : public IDataAbility
{/**
	二回攻撃/一気呵成
*/
public:
	// コンストラクタ
	CAbility_TwiceAttack(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

