#include "stdafx.h"

#include "../../Demo/CDemoMovieClip.h"
#include "CMapSymbolDB.h"

namespace BMW{
namespace SLG{

Task::ITaskBase* CMapSymbolDB::createSymbolStr(const string& sID)
{
//	int nSymbol = symbolID_.getValue(sID);

/*
#ifdef BMW_DEBUG
	CDbg().Out("%d %s",nSymbol,sID.c_str());
#endif
*/
/*	if(sID=="ATK" 
	|| sID=="DAMAGE"
	|| sID=="DEFENCE"
	|| sID=="AVOID_L"
	|| sID=="AVOID_R"
	|| sID=="POWERUP")
	{
		Movie::CMovieClip* pClip = new Demo::CDemoMovieClip();
		setMovieClip(pClip, getSymbolData(nSymbol));
		return pClip;
	}
*/
	return CSymbolDB::createSymbolStr(sID);
}

} // namespace SLG end
} // namesapce BMW end