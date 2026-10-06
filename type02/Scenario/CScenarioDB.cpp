#include "stdafx.h"

#include "CDataScenario.h"
#include "CScenarioParser.h"
#include "CScenarioDB.h"

namespace BMW{
namespace Scenario{

CScenarioDB::~CScenarioDB()
{
	scenario_map::iterator it;
	for(it=mapScenario_.begin(); it!=mapScenario_.end(); it++)
		DELETE_SAFE(it->second);

	mapScenario_.clear();
}

void CScenarioDB::setScenarioDB(const string& sFile)
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
	CScenarioParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full)	CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

int CScenarioDB::getNo(int nID)
{
	return getScenario(nID)->getNo();
}

const string& CScenarioDB::getTitle(int nID)
{
	return getScenario(nID)->getTitle();
}

} // namespace Scenario end
} // namespace BMW end