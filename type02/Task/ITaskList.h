/*
	katze 05/04/19
	update 06/02/01 親クラスをITaskBaseに
	タスクリストの基底クラス
*/
#pragma once

#include "ITaskBase.h"

namespace BMW{
namespace Task{

class CTaskContext;
class ITaskListCtrl;

class ITaskList : public ITaskBase
{/**
	タスクリストの基底クラス
 */
public:
	// コンストラクタ・デストラクタ
	ITaskList(){}
	ITaskList(const smart_ptr<ITaskListCtrl>& pC):pCtrl_(pC){}
	virtual ~ITaskList(){}

	// タスク処理
	virtual void Task(CTaskContext*);
	virtual void OnInit(CTaskContext*){}
	// returnListした時に呼ばれる
	virtual void OnComeBack(int nID,CTaskContext*){}
	virtual void callTaskReset(Task::CTaskContext*){}

	// 設定・取得
	smart_ptr<ITaskListCtrl>&	getTaskListCtrl(){ return pCtrl_; }
	void						setTaskListCtrl(const smart_ptr<ITaskListCtrl>& pCtrl){ pCtrl_=pCtrl; }

	// 操作
	virtual void		addTask(ITaskBase* pBase, int nPri){}
	virtual void		addTask(ITaskBase* pBase){}
	virtual void		killTask(int nPri){}
	virtual ITaskBase*	removeTask(int nPri){ return NULL; } // removeしたTaskBaseが返ってくる
	virtual void		clearTask(){}
	virtual ITaskBase*	getTask(int nPri){ return NULL; }

	virtual void killMe(){}
	virtual void removeMe(){}

	virtual bool IsKill() const { return false; }
	virtual void kill(bool bKill){}
	virtual bool IsRemove() const { return false; }
	virtual void remove(bool bRemove){}

protected:
	// タスクリストのTaskを呼び出す
	virtual void callTaskAction(CTaskContext*){}	// 動作用
	virtual void callTaskDraw(CTaskContext*){}		// 描画用

	// タスクListコントローラ
	smart_ptr<ITaskListCtrl> pCtrl_;
};

} // namespace Task end
} // namespace BMW end