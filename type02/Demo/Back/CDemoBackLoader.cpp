#include "stdafx.h"

#include "CBackSymbolDB.h"
#include "CDemoBack.h"
#include "CDemoBackParser.h"
#include "CDemoBackLoader.h"

namespace BMW{
namespace Demo{

CDemoBackLoader::CDemoBackLoader()
{
	pSymbolDB_ = new CBackSymbolDB();
}

CDemoBackLoader::~CDemoBackLoader()
{
	DELETE_SAFE(pSymbolDB_);
}

CDemoBack* CDemoBackLoader::createDemoBack(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG_DEMO
	CDbg().Out("GUIDEF %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	CDemoBack* pBack = new CDemoBack();

	// 構文解析
	CDemoBackParser ps(pSymbolDB_,pBack);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif

	return pBack;
}

} // namespace Demo end
} // namespace BMW end