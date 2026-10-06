#include "stdafx.h"

#include "CMap.h"
#include "CMapParser.h"
#include "CMapLoader.h"

namespace BMW{
namespace SLG{
namespace Map{

void CMapLoader::setMap(CMap* pMap, const string& sFile)
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
	CMapParser ps(pMap, sprite_);
	Parser::skip_comment skip;
	int nIndex=0;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps[var(nIndex)=arg1], skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",r.stop);
#endif
	pMap->scrollIndex(nIndex);
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end