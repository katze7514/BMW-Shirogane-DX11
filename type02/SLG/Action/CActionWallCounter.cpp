#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "IDAction.h"
#include "CActionWallCounter.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionWallCounter::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::WALL_COUNTER;
		s << nID;
		nID = 0;
		s << nID;
	}
}

void CActionWallCounter::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::WALL_COUNTER;
}

void CActionWallCounter::action(CDataCharaSLG& chara, CSLGContext& p)
{// 壁ルーチン。反撃はするがまったく動かない
	// 状況をリセット
	setUseWeapon(-1);	// 攻撃せず
	setMapIndex(-1);	// 移動もしない
}

int	CActionWallCounter::actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP, bool bBackUp)
{// 反撃できないなら、防御する
	if(chara.getBattle().IsCond(Chara::CValidCond::ACTION)) return Battle::HIT;
	int nAction = CActionSimple::actionCounter(chara,nDist,nRealDist,nHeight,context,nHP);
	if(nAction==Battle::HIT) return Battle::DEFENCE;
	return nAction;
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end