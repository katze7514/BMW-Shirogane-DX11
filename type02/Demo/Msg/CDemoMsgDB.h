/*
	katze 05/07/05
	デモMsgDB
*/
#pragma once

namespace BMW{
namespace Demo{
class CDemoContext;
class CDemoMsgList;

class CDemoMsgDB
{/**
	デモMsgDB
 */
public:
	typedef map<int, CDemoMsgList*> msg_map;
	
	// デストラクタ
	~CDemoMsgDB();

	// 設定
	void setDemoMsg(const string& sFile,CDemoContext* pContext);

	// 操作
	void			addDemoMsgList(int nID, CDemoMsgList* pMsg){ mapMsg_.insert(pair<int, CDemoMsgList*>(nID,pMsg)); }
	CDemoMsgList*	getDemoMsgList(const string& sID){ return getDemoMsgList(msgID_.getValue(sID)); }
	CDemoMsgList*	getDemoMsgList(int nID)
	{ 
		if(0<=nID && nID<static_cast<int>(mapMsg_.size()))
			return mapMsg_[nID];
		else
			return NULL;
	}

	int				getDemoMsgID(const string& sID){ return msgID_.getValue(sID); }
	void			setDemoMsgID(const string& sID,int nID){ msgID_.writeMap(sID,nID); }

private:
	msg_map	 mapMsg_;
	katzeSDK::Misc::CStringMap msgID_;
};

} // namespace Demo end
} // namespace BMW end