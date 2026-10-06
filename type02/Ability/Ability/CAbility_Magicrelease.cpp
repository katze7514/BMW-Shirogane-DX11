#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Context/COffsetBattle.h"

#include "CAbility_Magicrelease.h"

namespace BMW{
namespace Ability{
////////////////////////////////////////////
// 補正
////////////////////////////////////////////
void CAbility_Magicrelease::applyOffset(SLG::COffsetBattle& data, int nAttack, int nTough)
{
	// 攻撃力とToughに補正
	data.calcAttack(nAttack/10);
	data.calcTough(nTough/10);
}

/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Magicrelease::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// nAttrはこいつを使うまでに使用予定のEN値
	return slg.getBattle().getEN()>=getEN()+nAttr;
}

} // namespace Ability end
} // namespace BMW end
