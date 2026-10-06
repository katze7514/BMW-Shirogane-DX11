/*
	katze 05/03/09
	戦闘用状態変化武器
*/
#pragma once

#include "CDataWeaponBattle.h"

namespace BMW{
namespace Weapon{

class CDataWeaponBattleCond : public CDataWeaponBattle
{/**
	戦闘用状態変化武器を表現するクラス
 */
public:
	// デストラクタ
	virtual ~CDataWeaponBattleCond(){}
	// 効果適用
	void	apply(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);
};

} // namespace Weapon end
} // namespace BMW end