#include "stdafx.h"

#include "CBattleEvent.h"
#include "CSlgBattleEventParser.h"
#include "CSlgBattleEventLoader.h"

namespace BMW{
namespace SLG{

Event::CBattleEventData* CSlgBattleEventLoader::createBattleEvent(const string& sData)
{
	using namespace boost::spirit;
	using namespace phoenix;

	Event::CBattleEventData* pBattle = new Event::CBattleEventData();
	// ç\ï∂âêÕ
	CSlgBattleEventParser ps;
	ps.setBattleEvent(pBattle);
	ps.setSlgDef(pDef_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s ì«Ç›çûÇ›é∏îsÅIÅI",/*sFile.c_str()*/r.stop);
#endif

	return pBattle;
}

} // namespace SLG end
} // namespace BMW end