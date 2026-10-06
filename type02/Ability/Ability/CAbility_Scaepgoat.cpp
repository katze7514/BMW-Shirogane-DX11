#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Scaepgoat.h"

namespace BMW{
namespace Ability{
/////////////////////////////////////////////
// 使用
/////////////////////////////////////////////
bool CAbility_Scaepgoat::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 気力130以上で発動。50%確率で生き残る
	return slg.getBattle().getMental() >=130
		&& slg.getBattle().getEN() >= (getEN()+nAttr) 
		&& CApp::rand_.Get(100)+1 <= 50;
}

} // namespace Ability end
} // namespace BMW end
