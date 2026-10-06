#include "stdafx.h"

#include "CSpriteDB.h"

namespace BMW{
namespace Draw{

#ifdef BMW_DEBUG
//#define BMW_DEBUG_SPRITE
#endif

void CSpriteDB::setSpriteDB(const string& sFile)
{
	setSpriteDBPre(sFile,"");
}

void CSpriteDB::setSpriteDBPre(const string& sFile, const string& sPre)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG
	//CDbg().Out("SPRITE %s",sFile.c_str());
#endif

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	setSpriteStream(p,sPre);

#ifdef BMW_DEBUG_SPRITE
	CDbg().Out("SPRITE END");
#endif
}

void CSpriteDB::setSpriteStream(const string& sData, const string& sPre)
{
	// 構文解析
	CSpriteParser ps(*this, sPre);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("SPRITE %s 読み込み失敗！！",r.stop);
#endif
}

void CSpriteDB::clearSpriteDB()
{
	mapSprite_.clear();
	loader_.ReleaseAll();
	mapID_.clearMap();
}

void CSpriteDB::addSpriteDataStr(const CSpriteInfoBase& base, const string& sID)
{
#ifdef BMW_DEBUG
	if(getSpriteID(sID)>0)
		CDbg().Out("SPRITE %s はすでに存在します",sID.c_str());
#endif
	setSpriteID(sID);
	mapSprite_.insert(pair<int,CSpriteInfoBase>(getSpriteID(sID),base));
}

void CSpriteDB::getSpriteData(const string& sID, CSpriteInfoBase& base)
{
#ifdef BMW_DEBUG_SPRITE
	// みつかんない！？
	if(!getSpriteBase(getSpriteID(sID) ,base)) CDbg().Out("SPRITE %s が見つかりません",sID.c_str());
#else
	getSpriteBase(getSpriteID(sID), base);
#endif
}

CSpriteInfo	CSpriteDB::getSprite(int nID)
{
	CSpriteInfoBase base;
#ifdef BMW_DEBUG_SPRITE
	// みつかんない！？
	if(!getSpriteBase(nID, base)) CDbg().Out("SPRITE %d が見つかりません",nID);
#else
	getSpriteBase(nID,base);
#endif

	CSpriteInfo info;
	info.setPlane(loader_.GetPlane(base.getGraphicID()));
	info.setRect(base.getRect());
	info.setOffsetPos(base.getX(),base.getY());
	return info;
}

CSpriteInfo CSpriteDB::getSprite(const string& sID)
{
	CSpriteInfoBase base;
#ifdef BMW_DEBUG_SPRITE
	// みつかんない！？
	if(!getSpriteBase(getSpriteID(sID),base)) CDbg().Out("SPRITE %s が見つかりません",sID.c_str());
#else
	getSpriteBase(getSpriteID(sID), base);
#endif

	CSpriteInfo info;
	info.setPlane(loader_.GetPlane(base.getGraphicID()));
	info.setRect(base.getRect());
	info.setOffsetPos(base.getX(),base.getY());
	return info;
}

void CSpriteDB::setSprite(CSpriteInfo& info, int nID)
{
	info = getSprite(nID);
}

void CSpriteDB::setSprite(CSpriteInfo& info, const string& sID)
{
	info = getSprite(sID);
}

__inline bool CSpriteDB::getSpriteBase(int nID, CSpriteInfoBase& base)
{
	sprite_map::iterator it = mapSprite_.find(nID);

	if(it != mapSprite_.end())
	{
		base = it->second;
		return true;
	}
	else
	{	return false; }
}

} // namespace Draw end
} // namespace BMW end