#include "stdafx.h"

#include "CSlgEffectScriptParser.h"
#include "CSlgEffectScriptLoader.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CSlgEffectScriptLoader::createEffectScript(const string& sData, VM::CScript* pScript)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// ç\ï∂âêÕ
	CSlgEffectScriptParser ps(p_,pScript);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s ì«Ç›çûÇ›é∏îsÅIÅI",/*sFile.c_str()*/r.stop);
#endif
}

} // namepsace Effect end
} // namespace SLG end
} // namespace BMW end