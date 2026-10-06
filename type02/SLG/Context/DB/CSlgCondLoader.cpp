#include "stdafx.h"

#include "../CSLGDef.h"
#include "CSlgCondList.h"
#include "CSlgCondParser.h"
#include "CSlgCondLoader.h"

namespace BMW{
namespace SLG{

ISlgCond* CSlgCondLoader::createCond(const string& sData)
{
	using namespace boost::spirit;
	using namespace phoenix;

	CSlgCondList* pList = new CSlgCondList();
	// ç\ï∂âêÕ
	CSlgCondParser ps;
	ps.setCondList(pList);
	ps.setSlgDef(pDef_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s ì«Ç›çûÇ›é∏îsÅIÅI",/*sFile.c_str()*/r.stop);
#endif

	return pList;
}

} // namespace SLG end
} // namespace BMW end