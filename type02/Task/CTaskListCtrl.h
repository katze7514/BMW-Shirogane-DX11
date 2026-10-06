/*
	katze 05/02/18
	タスクListコントローラの一番簡単な実装
*/
#pragma once

#include "ITaskListCtrl.h"

namespace BMW{
namespace Task{

class CTaskContext;

class CTaskListCtrl : public ITaskListCtrl
{/**
	タスクListコントローラの一番簡単な実装クラス
 */
public:
	// コンストラクタ・デストラクタ
	CTaskListCtrl(){}
	CTaskListCtrl(const smart_ptr<ITaskListFactory>& pFactory):pFactory_(pFactory){}
	virtual ~CTaskListCtrl(){}

	// タスク処理
	virtual void OnAction(CTaskContext*);
	virtual void OnDraw(CTaskContext*);

	// 設定・取得
	smart_ptr<ITaskListFactory>&	getTaskListFactory(){ return pFactory_; }
	void							setTaskListFactory(const smart_ptr<ITaskListFactory>& pFactory){ pFactory_=pFactory;}
	
	// 操作
	virtual void callTaskList(int nNextID,bool bFast=false){ setNextTaskList(nNextID); setState(bFast?MES_CALL_FAST:MES_CALL); }
	virtual void returnTaskList(){ setState(MES_RETURN); }
	virtual void jumpTaskList(int nNextID){	setNextTaskList(nNextID); setState(MES_JUMP); }
	virtual void exitTaskList(){ setState(MES_EXIT); }

	// 現在の動作TaskList情報を取得
	int						getCurrentTaskListID();
	smart_ptr<ITaskList>&	getCurrentTaskList();
	bool					IsEnd() const { return vecList_.empty(); }

protected:
	// TaskListファクトリ
	smart_ptr<ITaskListFactory> pFactory_;
	
	// 次に移動するTaskListID
	int nNextListID_;
	int	getNextTaskList(){ return nNextListID_; }
	void setNextTaskList(int nNext){ nNextListID_=nNext; }

	// TaskListスタック管理クラス
	class CTaskListInfo{
	public:
		CTaskListInfo():nTaskListID_(-1){}

		int nTaskListID_;
		smart_ptr<ITaskList> pTaskList_;
	};
	// スタック
	smart_vector_ptr<CTaskListInfo> vecList_;

	//内部利用関数
	void createTaskList(int nID,CTaskContext*);
	void CurrentTask(CTaskContext*);
	virtual void noExist(CTaskContext*);
};

} // namespace Task end
} // namespace BMW end