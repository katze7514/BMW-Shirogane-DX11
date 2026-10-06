#include "stdafx.h"

#include "../../SLG/Context/CDataCharaSLG.h"

#include "../CDemoMovieClip.h"
#include "../CDemoSymbolDB.h"
#include "CDemoSymbolCond.h"


#include "../Msg/CDemoMsgDB.h"
#include "CDemoMsgCond.h"

#include "CDemoDefParser.h"
#include "CDemoDef.h"

namespace BMW{
namespace Demo{

CDemoDef::CDemoDef()
{
	pSymbolDB_ = new CDemoSymbolDB();
	pMsgDB_ = new CDemoMsgDB();
}

CDemoDef::~CDemoDef()
{
	for(int i=0; i<SYMBOL_END; ++i)
	{
		for_each(listSymbolCond_[i].begin(), listSymbolCond_[i].end(), DeleteObj());
		listSymbolCond_[i].clear();
	}

	DELETE_SAFE(pSymbolDB_);

	msg_cond_map::iterator it;
	for(it=mapMsgCond_.begin(); it!=mapMsgCond_.end(); ++it)
		DELETE_SAFE(it->second);

	mapMsgCond_.clear();

	DELETE_SAFE(pMsgDB_);
}

// 設定
void CDemoDef::setDemoDef(const string& sFile,CDemoContext* pContext)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG
	CDbg().Out("DEMO_DEF %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CDemoDefParser ps(*this,pContext);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("DEMO_DEF_END %s 読み込み失敗！！",/*sFile.c_str()*/r.stop);
#endif
}

//////////////////////////////////////////////
// 操作
//////////////////////////////////////////////
void CDemoDef::setMsgList(const string& sID, int nDamage, const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, Task::CTaskContext* pContext, bool bEvent)
{// 攻撃の時は、一連のリストになるので、これで設定
	// メッセージリストを取得
	int nMsgID;
	if(bEvent)
	{// イベントの時は直接取得	
		nMsgID = pMsgDB_->getDemoMsgID(sID);
	}
	else// イベントじゃない時
	{
		msg_cond_map::iterator it = mapMsgCond_.find(sID);
		if(it!=mapMsgCond_.end())
			nMsgID = it->second->getMsgID((chara.getBattle().getHP()-nDamage)*100/chara.getBattle().getMaxHP(), target.getCharaID(),	pContext);
		else
			nMsgID=-1;
	}
	// リスト取得
	CDemoMsgList* pList = pMsgDB_->getDemoMsgList(nMsgID);
	// リスト設定
	pSymbolDB_->setMsgList(pList);
}

namespace{
int symbolsID2ID(const string& sID)
{
	if(sID=="ATTACK")	return CDemoDef::ATTACK;
	ef(sID=="HIT")		return CDemoDef::HIT;
	ef(sID=="DEFENCE")	return CDemoDef::DEFENCE;
	ef(sID=="AVOID")	return CDemoDef::AVOID;
	else				return -1;
}
} // namespace end

int CDemoDef::getSymbolID(int nID, int nRatio)
{
	CDemoSymbolCond* pCond;
	// とりあえず、最初のは設定しておく
	pCond = *listSymbolCond_[nID].begin();
	list<CDemoSymbolCond*>::iterator it;
	for(it=listSymbolCond_[nID].begin(); it!=listSymbolCond_[nID].end(); ++it)
		if((*it)->getRatio()>=nRatio){ pCond=*it; break; }

	return pCond->getSymbolID();
}

CDemoMovieClip* CDemoDef::createMovieClip(const string& sID, int nRatio)
{
#ifdef BMW_DEBUG
	CDbg().Out(sID);
#endif
	int nSymbolID;
	// 渡ってきた取得したいIDに合わせて
	int nID=symbolsID2ID(sID);
	if(nID>=0) // ATTACKとか条件付けがある
		nSymbolID = getSymbolID(nID, nRatio);
	else
		nSymbolID = pSymbolDB_->getID(sID);

	
#ifdef BMW_DEBUG
	CDbg().Out(nSymbolID);
#endif
	return pSymbolDB_->createDemoSymbol(nSymbolID);
}

} // namespace Demo end
} // namespace BMW end