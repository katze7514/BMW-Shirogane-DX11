#include "stdafx.h"

#include "../IDRule.h"
#include "CSLGDef.h"
#include "DB/CBattleEvent.h"
#include "DB/CSlgParser.h"
#include "DB/ISlgCond.h"

namespace BMW{
namespace SLG{

CSLGDef::CSLGDef():nFactory_(-1)
{// callで呼ばれる可能性のあるAPIをあらかじめ入れておく
	scriptID_.writeMap("VICTORY_VIEW",Rule::VICTORY_VIEW);
	scriptID_.writeMap("ATTACK_DEL",Rule::ATTACK_DEL);
	scriptID_.writeMap("PENALTY",Rule::PENALTY_RULE);
	scriptID_.writeMap("MENU_SELECT",Rule::MENU_SELECT);

	// 空呼び用
	scriptID_.writeMap("NONE", -1);
	// NullDevice
	smart_ptr<VM::CScript> pScript(new VM::CScript);
	pScript->addCode(new BMW::VM::Code::CCode_ret());
	mapScript_.insert(pair<int, smart_ptr<VM::CScript> >(-1,pScript));
}

CSLGDef::~CSLGDef()
{
	battle_map::iterator it;
	for(it=mapBattle_.begin(); it!=mapBattle_.end(); ++it)
		DELETE_SAFE(it->second);
	mapBattle_.clear();
	for_each(victoryCond_.begin(),victoryCond_.end(),DeleteObj());
	victoryCond_.clear();
	for_each(loseCond_.begin(),loseCond_.end(),DeleteObj());
	loseCond_.clear();
	for_each(expertCond_.begin(),expertCond_.end(),DeleteObj());
	expertCond_.clear();
}

void CSLGDef::setSLGDef(const string& sFile, CSLGContext* pContext)
{
	using namespace boost::spirit;
	using namespace phoenix;
#ifdef BMW_DEBUG
	CDbg().Out("SLG_DEF %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CSlgParser ps(*this,pContext);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",/*sFile.c_str()*/r.stop);
#endif
}

smart_ptr<VM::CScript>& CSLGDef::getScript(int nID)
{ 
	script_map::iterator it = mapScript_.find(nID);
	if(it!=mapScript_.end()) return it->second;
#ifdef BMW_DEBUG
	CDbg().Out("SLG FUN %d が見つかりません", nID);
#endif
	// 見つからなかったらNullDevice
	return mapScript_.find(-1)->second;
}

void CSLGDef::setScriptID(const string& sID)
{// IDだけ予約する
	scriptID_.writeMap(sID, Rule::USER_DEF+scriptID_.getMapSize()); 
}

void CSLGDef::setScript(const string& sID, VM::CScript* pScript)
{	
	int nID = getScriptID(sID);
	if(nID<0)
	{// 予約されてなかったら、ID確保
		nID = Rule::USER_DEF+scriptID_.getMapSize();
		scriptID_.writeMap(sID,nID);
	}
	mapScript_.insert(pair<int, smart_ptr<VM::CScript> >(nID,smart_ptr<VM::CScript>(pScript)));
}

Event::CBattleEventData* CSLGDef::getBattleEvent(int nID)
{
	battle_map::iterator it = mapBattle_.find(nID);
	if(it!=mapBattle_.end()) return it->second;
#ifdef BMW_DEBUG
	CDbg().Out("BattleEvent %s が見つかりません", nID);
#endif
	// 見つからなかったらNullDevice
	return NULL;
}

Event::CBattleEventData* CSLGDef::getBattleEvent(const string& sID)
{
	return getBattleEvent(battleID_.getValue(sID));
}

void CSLGDef::setBattleEvent(const string& sID, Event::CBattleEventData* pBattle)
{
	battleID_.writeMap(sID,battleID_.getMapSize());
	mapBattle_.insert(pair<int,Event::CBattleEventData*>(battleID_.getValue(sID),pBattle));
}

bool CSLGDef::IsVictory(CSLGContext* pContext)
{
	return IsCond(victoryCond_, pContext->getVictory(), pContext);
}

bool CSLGDef::IsLose(CSLGContext* pContext)
{
	return IsCond(loseCond_, pContext->getLose(), pContext);
}

bool CSLGDef::IsExpert(CSLGContext* pContext)
{
	return IsCond(expertCond_, pContext->getExpertCond(), pContext);
}

bool CSLGDef::IsCond(vic_vec& vec, int nID, CSLGContext* pContext)
{
	if(nID<0 || nID>=(int)vec.size()) return false;
	return vec[nID]->judg(pContext);
}

} // namespace SLG end
} // namespace BMW end