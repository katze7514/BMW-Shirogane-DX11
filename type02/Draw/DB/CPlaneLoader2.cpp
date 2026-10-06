#include "stdafx.h"

#include "CPlaneParser.h"
#include "CPlaneLoader2.h"

namespace BMW{
namespace Draw{

#ifdef BMW_DEBUG
//#define BMW_DEBUG_PLANE
#endif

LRESULT	CPlaneLoader2::Set(const string& filename, bool bUseID)
{
	return SetPre(filename,"",bUseID);
}

LRESULT	CPlaneLoader2::SetPre(const string& filename, const string& sPre, bool bUseID)
{
	if(!bUseID && !m_bCanReloadAgain) return 0;

	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG
	//CDbg().Out("PLANE %s",filename.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	if (file.Read(filename)!=0) return 1;
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CPlaneParser ps(*this,sPre);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full){ CDbg().Out("PL %s  読み込み失敗！！", filename.c_str()); CDbg().Out(r.stop); return 2; }
#endif
	m_bCanReloadAgain=false;

#ifdef BMW_DEBUG_PLANE
	CDbg().Out("PLANE END");
#endif

	return 0;
}

LRESULT	CPlaneLoader2::ReleaseAll()
{
	m_bCanReloadAgain=true;
	mapID_.clearMap();
	return CPlaneLoader::ReleaseAll();
}

} // namespace Draw end
} // namespcae BMW end