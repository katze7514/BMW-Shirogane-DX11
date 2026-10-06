/*
	katze 05/03/09
	戦闘用ステータス変化武器
*/
#pragma once

#include "CDataWeaponBattle.h"

namespace BMW{
namespace Weapon{

class CDataWeaponBattleStatus : public CDataWeaponBattle
{/**
	戦闘用ステータス変化武器を表現するクラス
 */
public:
	// デストラクタ
	virtual ~CDataWeaponBattleStatus(){}
	// 使用可能
	bool	enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context);
	// 操作
	void	apply(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);
};

} // namespace Weapon end
} // namespace BMW end