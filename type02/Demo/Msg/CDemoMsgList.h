/*
	katze 05/07/05
	デモMsgDBの要素
*/
#pragma once

namespace BMW{
namespace Demo{
class CDemoMsg;

class CDemoMsgList
{/**
	メッセージリスト
 */
public:
	// デストラクタ
	~CDemoMsgList();

	// 操作
	void		resizeMsgList(int nSize){ vecMsg_.resize(nSize); }
	CDemoMsg*	getMsg(int nID){ return vecMsg_[nID]; }
	void		setMsg(int nID, CDemoMsg* pMsg){ vecMsg_[nID]=pMsg; }

private:
	vector<CDemoMsg*>	vecMsg_;
};

} // namespace Demo end
} // namespace BMW end