/*
	katze 05/12/27
	update 06/04/02 Prefixがつけられるように（注！でも、ちゃんと動いてない。とりあえず、保留）
	スプライトDB ver.2
*/
#pragma once

#include "../CSpriteInfoBase.h"
#include "CPlaneLoader2.h"

namespace BMW{
namespace Draw{

class CSpriteInfo;
class CSpriteDB
{/**
	スプライトDB
 */
public:
	typedef map<int, CSpriteInfoBase> sprite_map;

	// 操作
	void		setSpriteDB(const string& sFile);
	void		setSpriteDBPre(const string& sFile, const string& sPre);
	void		setSpriteStream(const string& sData, const string& sPre="");
	void		clearSpriteDB();
	
	int			getSpriteID(const string& sID) const
				{ 
				/*#ifdef BMW_DEBUG
					if(mapID_.getValue(sID)<0) 
						CDbg().Out("NO SPRITE ID %s",sID.c_str());
				#endif*/
					return mapID_.getValue(sID);
				}
	void		setSpriteID(const string& sID, int nID){ mapID_.writeMap(sID,nID); }
	void		setSpriteID(const string& sID){ mapID_.writeMap(sID,mapID_.getMapSize()); }

	void		addSpriteData(const CSpriteInfoBase& base, int nID)
	{ 
		mapSprite_.insert(pair<int,CSpriteInfoBase>(nID,base));
	}
	void		getSpriteData(const string& sID, CSpriteInfoBase& base);

	void		addSpriteDataStr(const CSpriteInfoBase& base, const string& sID);

	CSpriteInfo	getSprite(int nID);
	CSpriteInfo	getSprite(const string& sID);

	void		setSprite(CSpriteInfo& info, int nID);
	void		setSprite(CSpriteInfo& info, const string& sID);

	CPlaneLoader2&	getLoader(){ return loader_; }

private:
	CPlaneLoader2	loader_;
	sprite_map		mapSprite_;

	katzeSDK::Misc::CStringMap	mapID_;

	// 内部利用
	bool getSpriteBase(int nID, CSpriteInfoBase& base);
};

} // namespace Draw end
} // namespace BMW end