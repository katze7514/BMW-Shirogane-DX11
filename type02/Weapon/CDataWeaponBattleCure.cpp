#include "stdafx.h"

#include "../SLG/IDSLG.h"
#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "../SLG/Map/CMapChip.h"
#include "../SLG/Map/CMapChipState.h"

#include "CDataWeaponBattleCure.h"

namespace BMW{
namespace Weapon{

bool CDataWeaponBattleCure::enableRange(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, int nDist, int nRealDist,int nHeight)
{
	if(nDist<0)
	{// 距離が指定されてないなら全検索
		// 使えるなら、範囲内に回復できるキャラがいるかチェック
		for(int i=getMin(); i<=getMax(); ++i)
			if(enableRangeOne(i-1,nHeight,context,chara)) return true;
	}
	else
	{// されてるなら、そこだけ
		if(enableRangeOne(nDist-1,nHeight,context,chara)) return true;
	}
	
	return false;
}

bool CDataWeaponBattleCure::enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context)
{
	return target.IsLive() && chara.getPhase()==target.getPhase() && enableTargetOne(target,context);
}

bool CDataWeaponBattleCure::enableTargetOne(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context)
{
	return chara.getBattle().getBattleOffset().getHP()>0;
}

bool CDataWeaponBattleCure::enableRangeOne(int nRange, int nHeight, SLG::CSLGContext& context, const SLG::CDataCharaSLG& chara)
{
	SLG::CSLGContext::range_list& listRange = context.getRangeList(nRange);
	SLG::CDataCharaSLG* pChara;
	int nAtkHeight;
	SLG::CSLGContext::range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
	{
		if(it->nID_<0) continue;
		nAtkHeight = nHeight>0 ? nHeight : context.getMapChip(it->nIndex_)->getMapChipState()->getAtkHeight();
		if(nAtkHeight<=getHeight())
		{// 攻撃可能な高さの差
			pChara = context.getCharaData(it->nID_);
			if(enableTarget(chara,*pChara,context)) return true;
		}
	}
	return false;
}

} // namespace Weapon end
} // namespace BMW end