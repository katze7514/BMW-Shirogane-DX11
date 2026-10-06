#include "stdafx.h"

#include "DB/CFaceDB.h"
#include "ConstFace.h"
#include "CFaceMapParser.h"
#include "CFaceMap.h"

namespace BMW{
namespace Face{

CFaceMap::~CFaceMap()
{
	facedb_map::iterator it;
	for(it=mapFace_.begin(); it!=mapFace_.end(); ++it)
		DELETE_SAFE(it->second);

	mapFace_.clear();
}

void CFaceMap::setFaceMap(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	//for(string::iterator it = p.begin(); it!=p.end(); ++it)
	//CDbg().Out(p);

	// 構文解析
	CFaceMapParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full)	CDbg().Out("%sの読み込み失敗！！",sFile.c_str());
#endif
}

const string& CFaceMap::getName(int nID)
{
	return mapFace_[nID]->getName();
}

void CFaceMap::addEqFaceMap(int nFaceID, int nEQ)
{
	faceeq_map::iterator it=mapEqFace_.find(nFaceID);
	if(it!=mapEqFace_.end())
	{// 等価セットがあれば、追加
		it->second.insert(nEQ);
	}
	else
	{// なければ、新規追加
		set<int> s;
		s.insert(nEQ);
		mapEqFace_.insert(pair<int, set<int> >(nFaceID,s));
	}
}

bool CFaceMap::IsFace(int nFaceID, int nEQ)
{
	// 同じだったら、もちろんtrue
	if(nFaceID==nEQ) return true;
	// 違うなら一応等価チェック
	faceeq_map::iterator it=mapEqFace_.find(nFaceID);
	// 等価セットないなら、false
	if(it==mapEqFace_.end()) return false;
	// あったら、中身拝見
	return it->second.find(nEQ)!=it->second.end();
}

////////////////////////////////////
// グラフィック設定
////////////////////////////////////
void CFaceMap::setNameGraphic(GUI::CGraphic* pGraphic, int nID, int nX, int nY)
{
	mapFace_[nID]->setNameGraphic(pGraphic, nX, nY);
}

void CFaceMap::setFaceGraphic(GUI::CGraphic* pGraphic, int nID, int nFaceID, int nToward, int nX, int nY, bool bBattle)
{
	mapFace_[nID]->setFaceGraphic(pGraphic, nFaceID, nToward, nX, nY);
	Draw::CSpriteInfo& info = const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo());
	if(bBattle)
	{// 戦闘用だったら、RECTをいじる
		RECT& rect = const_cast<RECT&>(info.getRect());
		rect.left += info.getX();
		rect.right = rect.left + Const::FACE_SIZE.cx;
		rect.top += info.getY();
		rect.bottom = rect.top + Const::FACE_SIZE.cy;
		info.setRect(rect);
	}
	// 実際に描画する時は、offsetはいらない
	info.setOffsetPos(0,0);
}

void CFaceMap::setFaceGraphic(GUI::CGraphic* pGraphic, int nID, const string& sFaceID, int nToward, int nX, int nY, bool bBattle)
{
	mapFace_[nID]->setFaceGraphic(pGraphic, sFaceID, nToward, nX, nY);
	Draw::CSpriteInfo& info = const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo());
	if(bBattle)
	{// 戦闘用だったら、RECTをいじる
		RECT& rect = const_cast<RECT&>(info.getRect());
		rect.left += info.getX();
		rect.right = rect.left + Const::FACE_SIZE.cx;
		rect.top += info.getY();
		rect.bottom = rect.top + Const::FACE_SIZE.cy;
		info.setRect(rect);
	}
	// 実際に描画する時は、offsetはいらない
	info.setOffsetPos(0,0);
}

} // namespace Face end
} // namesapce BMW end