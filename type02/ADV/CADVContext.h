/*
	katze 05/05/18
	ADVコンテキスト
*/
#pragma once

#include "DB/CBackDB.h"

#include "CADVMsgLog.h"

namespace BMW{
namespace ADV{
class CADVBack;
class CMsgBoard;

class CADVContext : public Task::CTaskContext
{/**
	ADVコンテキスト
 */
public:
	// 設定・取得
	CBackDB&				getBackDB(){ return backDB_; }
	void					setBackDB(const string& sFile){ backDB_.setBackDB(sFile); }

	smart_ptr<CADVBack>&	getBack(){ return pBack_; }
	void					setBack(const smart_ptr<CADVBack>& pBack){ pBack_=pBack; }

	smart_ptr<CMsgBoard>&	getMsgBoard(int nSide){ return pMsgBoard_[nSide]; }
	void					setMsgBoard(const smart_ptr<CMsgBoard>& pMsgBoard, int nSide){ pMsgBoard_[nSide]=pMsgBoard; }

	list<CADVMsgLog>&		getBackLogList(){ return listBackLog_; }
	void					addBackLog(int nSide, int nChara, int nFace, int nString, int nMask){	listBackLog_.push_front(CADVMsgLog(nSide,nChara,nFace,nString,nMask));	}
	void					clearBackLog(){ listBackLog_.clear(); }

private:
	// 背景
	smart_ptr<CADVBack>	pBack_;
	// 背景DB
	CBackDB backDB_;

	// メッセージボード
	smart_ptr<CMsgBoard> pMsgBoard_[2];
	// バックログ用リスト
	// 前に前にとデータが追加されていく
	// つまり、先頭が最新のログ
	list<CADVMsgLog> listBackLog_;
};

} // namespace ADV end
} // namespace BMW end