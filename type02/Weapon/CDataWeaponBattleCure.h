/*
	katze 06/06/27
	治療武器
*/
#pragma once

#include "CDataWeaponBattle.h"

namespace BMW{
namespace Weapon{

class CDataWeaponBattleCure : public CDataWeaponBattle
{/**
	治療武器
 */
public:
	// デストラクタ
	virtual ~CDataWeaponBattleCure(){}
	// 操作
	virtual bool enableRange(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, int nDist=-1, int nRealDist=-1,int nHeight=-1);
	bool		 enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context);

	virtual bool enableTargetOne(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);
	bool		 enableRangeOne(int nRange, int nHeight, SLG::CSLGContext& context, const SLG::CDataCharaSLG& chara);
};

} // namespace Weapon end
} // namespace BMW end