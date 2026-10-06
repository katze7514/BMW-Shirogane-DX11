/*
	katze 05/03/24
	update 07/02/16
	顔DB Ver.2
*/
#pragma once

#include "../../Draw/DB/CSpriteDB.h"
namespace BMW{ namespace GUI{ class CGraphic; } }
#include "../../Draw/CSpriteInfo.h"

namespace BMW{
namespace Face{

class CFaceDB
{/**
	顔DB

	Face定義ファイル一つがこいつ一つに対応する
 */
public:
	typedef map<int, CDataFace> face_map;

	// 操作
	void setFaceDB(const string& sFile);

	const string&		getName() const { return sName_; }
	void				setName(const string& sName){ sName_=sName; }

	const CDataFace&	getFaceData(int nID)
	{
	#ifdef BMW_DEBUG
		if(mapFace_.find(nID)==mapFace_.end()) Err.Out("not Face %s: %d", sName_.c_str(), nID);
	#endif
		return mapFace_[nID];
	}
	const CDataFace&	getFaceData(const string& sID)
	{ 
	#ifdef BMW_DEBUG
		if(mapFace_.find(mapID_.getValue(sID))==mapFace_.end()) Err.Out("not Face %s: %s", sName_.c_str(), sID.c_str());
	#endif

		return getFaceData(mapID_.getValue(sID));
	}
	void						addFaceData(const CDataFace& face, int nID){ mapFace_[nID]=face; }
	void						writeFaceIDMap(const string& sID, int nID){ mapID_.writeMap(sID,nID); }
	katzeSDK::Misc::CStringMap& getFaceStringIDMap(){ return mapID_; }

	int					getFaceID(const string& sID) const
						{ 
						#ifdef BMW_DEBUG
							if(mapID_.getValue(sID)<0) CDbg().Out((LPSTR)"not FaceID %s: %s", sName_.c_str(), sID.c_str());
						#endif
							return mapID_.getValue(sID);
						}

	// スプライトを直接取得するヘルパ
	Draw::CSpriteInfo	getFaceSprite(int nID, int nToward)
						{ return sprite_.getSprite(getFaceData(nID).getFaceID(nToward)); }
	Draw::CSpriteInfo	getFaceSprite(const string& sID, int nToward)
						{ return sprite_.getSprite(getFaceData(sID).getFaceID(nToward)); }

	// 良く使うであろうCGraphic向け特殊化
	void				setNameGraphic(GUI::CGraphic* pGraphic, int nX=0, int nY=0);
	void				setFaceGraphic(GUI::CGraphic* pGraphic, int nID, int nToward, int nX=0, int nY=0);
	void				setFaceGraphic(GUI::CGraphic* pGraphic, const string& sID, int nToward, int nX=0, int nY=0);

private:
	string			sName_;		// このFaceDBに対応するキャラ名
	Draw::CSpriteDB sprite_;
	face_map		mapFace_;

	katzeSDK::Misc::CStringMap	mapID_;
};

} // namespace BMW end
} // namespace Face end
