#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "IDAction.h"
#include "CActionNo.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionNo::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::NOACT;
		s << nID;
		nID = 0;
		s<< nID;
	}
}

void CActionNo::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::NOACT;
}

void CActionNo::action(CDataCharaSLG& chara, CSLGContext& p)
{// 反撃はするがまったく動かない
	// 状況をリセット
	setUseWeapon(-1);	// 攻撃せず
	setMapIndex(-1);	// 移動もしない
}

int	CActionNo::actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP, bool bBackUp)
{
	return Battle::HIT;
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end