#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "IDAction.h"
#include "CActionWall.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionWall::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::WALL;
		s << nID;
		nID = 0;
		s<< nID;
	}
}

void CActionWall::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::WALL;
}

void CActionWall::action(CDataCharaSLG& chara, CSLGContext& p)
{// 壁ルーチン。反撃はするがまったく動かない
	// 状況をリセット
	setUseWeapon(-1);	// 攻撃せず
	setMapIndex(-1);	// 移動もしない
}

int	CActionWall::actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP, bool bBackUp)
{// 防御する
	// この時でも、行動不能は有効
	return chara.getBattle().IsCond(Chara::CValidCond::ACTION) ? Battle::HIT : Battle::DEFENCE;
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end