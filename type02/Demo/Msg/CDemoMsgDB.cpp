#include "stdafx.h"

#include "../CDemoContext.h"

#include "CDemoMsgList.h"
#include "CDemoMsgParser.h"

#include "CDemoMsgDB.h"

namespace BMW{
namespace Demo{

CDemoMsgDB::~CDemoMsgDB()
{
	msg_map::iterator it;
	for(it=mapMsg_.begin(); it!=mapMsg_.end(); it++)
		DELETE_SAFE(it->second);

	mapMsg_.clear();
}

void CDemoMsgDB::setDemoMsg(const string& sFile,CDemoContext* pContext)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG
	CDbg().Out("DEMO_MSG %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CDemoMsgParser ps(*this,pContext);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",/*sFile.c_str()*/r.stop);
#endif
}

} // namespace Demo end
} // namepace BMW end