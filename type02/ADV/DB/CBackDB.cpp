#include "stdafx.h"

#include "../ConstADV.h"

#include "CDataBack.h"
#include "CBackParser.h"

#include "CBackDB.h"

namespace BMW{
namespace ADV{

CBackDB::~CBackDB()
{
	back_map::iterator it;
	for(it=mapBack_.begin(); it!=mapBack_.end(); it++)
		DELETE_SAFE(it->second);

	mapBack_.clear();
}

void CBackDB::setBackDB(const string& sFile)
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
	CBackParser ps(*this,sprite_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",sFile.c_str());
#endif
}


CDataBack* CBackDB::getBackData(int nID)
{
	back_map::iterator it = mapBack_.find(nID);
	if(it==mapBack_.end()) return NULL;
	return it->second; 
}

CDataBack* CBackDB::getBackData(const string& sID)
{ 
	back_map::iterator it = mapBack_.find(Const::backID_.getValue(sID));
	if(it==mapBack_.end()) return NULL;
	return it->second; 
}

void CBackDB::setBackData(CDataBack* back, int nID)
{
	mapBack_.insert(pair<int, CDataBack*>(nID,back));
}

void CBackDB::setBack(GUI::CGraphic* pGraphic, GUI::CText* pText, int nID)
{// 渡ってきたIDに応じて、背景グラフィックと背景名を設定する
	CDataBack* pData = getBackData(nID);

#ifdef BMW_DEBUG
	CDbg().Out("BackID %d",nID);
#endif

	// スプライト設定
	sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),pData->getSpriteID());
	// 名前の設定
	pText->setText(pData->getName());
}

void CBackDB::setBack(GUI::CGraphic* pGraphic, GUI::CText* pText, const string& sID)
{
	setBack(pGraphic,pText,Const::backID_.getValue(sID));
}

} // namespace ADV end
} // namespace BMW end