#include "stdafx.h"

#include "../../BMW/IDBMW.h"
#include "../../BMW/BMWFactory.h"

#include "CSubroutineFactorySLG.h"
#include "CSLGFactory.h"

namespace BMW{
namespace SLG{

CSubroutineFactorySLG* CSLGFactory::createFactory(int nID)
{
	switch(nID)
	{
	// 第一部
	case Scenario::SLG::C_06:	return new C_06::CFactorySLG_C_06();

	// 第二部
	case Scenario::SLG::C_11:	return new C_11::CFactorySLG_C_11();
	case Scenario::SLG::T_20:	return new T_20::CFactorySLG_T_20();

	// デフォルト～
	default: return new CSubroutineFactorySLG();
	}
}

} // namespace SLG end
} // namespace BMW end