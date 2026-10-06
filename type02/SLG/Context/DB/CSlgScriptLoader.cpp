#include "stdafx.h"

#include "CSlgScriptParser.h"
#include "CSlgScriptLoader.h"

namespace BMW{
namespace SLG{

VM::CScript* CSlgScriptLoader::createScript(const string& sData)
{
	using namespace boost::spirit;
	using namespace phoenix;

	VM::CScript* pScript = new VM::CScript();
	// ç\ï∂âêÕ
	CSlgScriptParser ps(*pDef_,p_,pScript);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s ì«Ç›çûÇ›é∏îsÅIÅI",/*sFile.c_str()*/r.stop);
#endif

	return pScript;
}

} // namespace SLG end
} // namespace BMW end