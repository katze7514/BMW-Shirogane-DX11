#include "stdafx.h"

#include "CConfigParser.h"
#include "CConfigDB.h"

namespace BMW{
namespace Config{

void CConfigDB::setConfigDB(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CConfigParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r =
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

} // namespace Config end
} // namespace BMW end