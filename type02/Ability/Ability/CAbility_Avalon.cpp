#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "CAbility_Avalon.h"

namespace BMW{
namespace Ability{

/////////////////////////////////////////////
// 使用可能？
/////////////////////////////////////////////
bool CAbility_Avalon::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// EN足りて気力140以上、2分の1で発動
	return slg.getBattle().getEN()>=getEN()+nAttr
		&& slg.getBattle().getMental()>=140 
		&& CApp::rand_.Get(2)==0;
}

bool CAbility_Avalon::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// ていうか使える？ EN的に
	//if(nAttr<getEN()) return false;
	// 気力130以上でフィールド無効
	return slg.getBattle().getEN()>=getEN()+nAttr
		&& slg.getBattle().getMental()>=130;
}

///////////////////////////////////////////
// ステータス適用
///////////////////////////////////////////
void CAbility_Avalon::applyStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.getBattle().calcHP(-(slg.getBattle().getMaxHP())/10);
}

void CAbility_Avalon::backStatus(SLG::CDataCharaSLG& slg, int nAttr)
{
	// HP10%回復
	slg.getBattle().calcHP(slg.getBattle().getMaxHP()/10);
}


} // namespace Ability end
} // namespace BMW end
