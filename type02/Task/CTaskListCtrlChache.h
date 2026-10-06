/*
	katze 05/02/18
	タスクListをキャッシュするタスクリストCtrl
*/
#pragma once

#include "ITaskListCtrl.h"

namespace BMW{
namespace Task{

class CTaskContext;

class CTaskListCtrlChache : public ITaskListCtrl
{/**
	タスクListをキャッシュするTaskListコントローラ
 */
public:
	// コンストラクタ・デストラクタ
	CTaskListCtrlChache(){}
	CTaskListCtrlChache(const smart_ptr<ITaskListFactory>& pFactory):pFactory_(pFactory){}
	virtual ~CTaskListCtrlChache(){}

	// タスク処理
	virtual void					OnAction(CTaskContext*);
	virtual void					OnDraw(CTaskContext*);

	// 設定・取得
	smart_ptr<ITaskListFactory>&	getTaskListFactory(){ return pFactory_; }
	void							setTaskListFactory(const smart_ptr<ITaskListFactory>& pFactory){ pFactory_=pFactory;}
	
	// 操作
	virtual void					callTaskList(int nNextID,bool bFast=false){ setNextTaskList(nNextID); setState(bFast?MES_CALL_FAST:MES_CALL); }
	virtual void					returnTaskList(){ setState(MES_RETURN); }
	virtual void					jumpTaskList(int nNextID){ setNextTaskList(nNextID); setState(MES_JUMP); }
	virtual void					exitTaskList(){ setState(MES_EXIT); }

	// キャッシュ操作
	// TaskListIDを渡すとそれに応じたTaskListがキャッシュに置かれる
	smart_ptr<ITaskList>			createChache(int nID,CTaskContext*);

	// 現在の動作TaskList情報を取得
	int								getCurrentTaskListID(){ return IsEnd() ? -1 : vecList_.rbegin()->nTaskListID_; }
	smart_ptr<ITaskList>&			getCurrentTaskList(){ return pCurrentTaskList_; }
	bool							IsEnd() const { return vecList_.empty(); }

protected:
	// TaskListファクトリ
	smart_ptr<ITaskListFactory> pFactory_;
	
	// 次に移動するTaskListID
	int nNextListID_;
	int	getNextTaskList(){ return nNextListID_; }
	void setNextTaskList(int nNext){ nNextListID_=nNext; }

	// キャッシュ対応Info
	struct CTaskListChacheInfo{
		int		nTaskListID_;
		bool	bDelete_;		// deleteフラグ CALL呼びされた時trueになる

		CTaskListChacheInfo():nTaskListID_(-1),bDelete_(false){}
	};
	// スタック
	vector<CTaskListChacheInfo> vecList_;
	// キャッシュ
	map<int, smart_ptr<ITaskList> > mapChache_;
	// 現在実行中のタスクList
	smart_ptr<ITaskList> pCurrentTaskList_;

	//内部利用関数
	void createTaskList(int nID,CTaskContext*);
	void CurrentTask(CTaskContext*);
	virtual void noExist(CTaskContext*);
};

} // namespace Task end
} // namespace BMW end