#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "CAbility_Battlefollow.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Battlefollow::enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p)
{
	// ただし、HPが10の時は発動しない
	// 1相手との技量差+10%の確率で発動
	// 最低でも5%は残る
	int nCT = base.getBattle().getSkill() - target.getBattle().getSkill() + 10;
	if(nCT<5) nCT=5;
	return base.getBattle().getHP()>10 && (int)CApp::rand_.Get(100)+1<=nCT;
}

} // namespace Ability end
} // namespace BMW end
