#include "stdafx.h"

#include "CFaceDB.h"
#include "../../GUI/CGraphic.h"

namespace BMW{
namespace Face{

void CFaceDB::setFaceDB(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;

#ifdef BMW_DEBUG
	//CDbg().Out("FACE %s",sFile.c_str());
#endif

	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CFaceParser ps(*this, sprite_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full){ CDbg().Out("FA %s  読み込み失敗！！", sFile.c_str()); CDbg().Out(r.stop);}
	//CDbg().Out("FACE END");
#endif
}


void CFaceDB::setNameGraphic(GUI::CGraphic* pGraphic, int nX, int nY)
{
	sprite_.setSprite(
		const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),
		"NAME"
		);
	if(nX!=0) pGraphic->setX(nX);
	if(nX!=0) pGraphic->setY(nY);
}
void CFaceDB::setFaceGraphic(GUI::CGraphic* pGraphic, int nID, int nToward, int nX, int nY)
{
	sprite_.setSprite(
		const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),
		getFaceData(nID).getFaceID(nToward)
		);
	if(nX!=0) pGraphic->setX(nX);
	if(nX!=0) pGraphic->setY(nY);
}
void CFaceDB::setFaceGraphic(GUI::CGraphic* pGraphic, const string& sID, int nToward, int nX, int nY)
{
	sprite_.setSprite(
		const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),
		getFaceData(sID).getFaceID(nToward)
		);
	if(nX!=0) pGraphic->setX(nX);
	if(nX!=0) pGraphic->setY(nY);
}
} // namespace Face end
} // namespace BMW end