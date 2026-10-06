/*
	katze 05/04/26
	update 06/02/01 親クラスをITaskBaseに
	単純なタスクの切り替えを行う
*/
#pragma once

#include "ITaskBase.h"

namespace BMW{
namespace Task{

template<class TaskT = ITaskBase>
class CTaskCtrl : public ITaskBase
{/**
	単純なタスク切り替えを行う

	基本的に外からの切り替えを行う
	実行中のタスクは、こいつのStateで制御する

	内部に持っているやつが、これに触ることはできないし、
	切り替わったことを検知することもない。
	それをやりたいときは、ITaskListCtrl系を使うべし

	なお、管理するタスクを切り替える前に、親としては、
	このCtrlの親を使うので、先にこいつの親を設定しておく

	また、設定されたタスクの解体責任も受け持つ
 */
public:
	typedef map<int, TaskT*> task_map;
	// コンストラクタ・デストラクタ
	virtual ~CTaskCtrl(){ clearTask(); }

	// タスク
	virtual void OnReset(CTaskContext*);
	virtual void OnAction(CTaskContext*);
	virtual void OnDraw(CTaskContext*);

	// サイズ取得
	void getSize(LONG& nWidth, LONG& nHeight) const;
	void getDrawSize(LONG& nWidth, LONG& nHeight) const;

	// 管理タスクの操作
	virtual void		addTask(TaskT* pBase, int nID);
	virtual TaskT*		getTask(int nID){ return mapTask_[nID]; }
	virtual TaskT*		getCurrentTask(){ return mapTask_[getState()]; }
	virtual void		clearTask();

	// マップ取得
	task_map&			getTaskMap(){ return mapTask_; }
	const task_map&		getTaskMap()const{ return mapTask_; }

	// タスク取得のヘルパ
	template<class T>
	T* getTaskCast(int nID){ return static_cast<T*>(getTask(nID)); }

protected:
	task_map mapTask_;
};

template<class TaskT>
void CTaskCtrl<TaskT>::OnReset(CTaskContext* pContext)
{
	typename task_map::iterator it;
	for(it=mapTask_.begin(); it!=mapTask_.end(); it++)
		(it->second)->OnReset(pContext);
}

template<class TaskT>
void CTaskCtrl<TaskT>::OnAction(CTaskContext* pContext)
{
	mapTask_[getState()]->Task(pContext);
}

template<class TaskT>
void CTaskCtrl<TaskT>::OnDraw(CTaskContext* pContext)
{
	mapTask_[getState()]->Task(pContext);
}

template<class TaskT>
void CTaskCtrl<TaskT>::getSize(LONG& lWidth, LONG& lHeight) const
{
	typename task_map::const_iterator it = mapTask_.find(getState());
	(it->second)->getSize(lWidth, lHeight);
}

template<class TaskT>
void CTaskCtrl<TaskT>::getDrawSize(LONG& lWidth, LONG& lHeight) const
{
	typename task_map::const_iterator it = mapTask_.find(getState());
	(it->second)->getDrawSize(lWidth, lHeight);
}

template<class TaskT>
void CTaskCtrl<TaskT>::addTask(TaskT* pBase, int nID)
{
	pBase->setTaskPriority(nID);
	pBase->setParent(getParent());
	mapTask_[nID]=pBase;
}

template<class TaskT>
void CTaskCtrl<TaskT>::clearTask()
{
	typename task_map::iterator it;
	for(it=mapTask_.begin(); it!=mapTask_.end(); it++)
		DELETE_SAFE(it->second);

	mapTask_.clear();
}

} // namespace Task end
} // namespace BMW end