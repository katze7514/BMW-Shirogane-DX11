/*
	katze 06/06/27
	補給武器
*/
#pragma once

#include "CDataWeaponBattleCure.h"

namespace BMW{
namespace Weapon{

class CDataWeaponBattleRefill : public CDataWeaponBattleCure
{/**
	補給武器
 */
public:
	// デストラクタ
	virtual ~CDataWeaponBattleRefill(){}
	// 操作
	bool enableTargetOne(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);
};

} // namespace Weapon end
} // namespace BMW end