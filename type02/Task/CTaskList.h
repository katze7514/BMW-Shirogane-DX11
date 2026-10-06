/*
	katze 05/02/17
	タスクListクラス
*/
#pragma once

#include "ITaskList.h"

namespace BMW{
namespace Task{

class CTaskList : public ITaskList
{/**
	タスクリストを表現する。
	前回で言う、CTaskContoller・CPanel・ISceneTaskを
	統合した（一緒くたに使うための）クラス
 */
public:
	typedef list<ITaskBase*> tasklist;

	// コンストラクタ・デストラクタ
	CTaskList():bKill_(false),bRemove_(false){}
	CTaskList(const smart_ptr<ITaskListCtrl>& pC):ITaskList(pC),bKill_(false),bRemove_(false){}
	virtual ~CTaskList(){ clearTask(); }

	// 子タスクのOnResetを呼び出す
	virtual void callTaskReset(Task::CTaskContext*);

	// 操作
	void		addTask(ITaskBase* pBase, int nPri);
	// listTask_のサイズをタスクプライオリティとして、タスクリストの末尾に追加する
	void		addTask(ITaskBase* pBase);
	void		killTask(int nPri);
	ITaskBase*	removeTask(int nPri); // removeしたTaskBaseが返ってくる
	void		clearTask();
	ITaskBase*	getTask(int nPri);

	void killMe(){ bKill_=true; }
	void removeMe(){ bRemove_=true; }

	bool IsKill() const { return bKill_; }
	void kill(bool bKill){ bKill_=bKill; }
	bool IsRemove() const { return bRemove_; }
	void remove(bool bRemove){ bRemove_=bRemove; }

	// タスク取得のヘルパ
	template<class T>
	T* getTaskCast(int nPri){ return static_cast<T*>(getTask(nPri)); }


	// タスクリスト取得
	tasklist&	getTaskList(){ return listTask_; }

protected:
	// タスクリスト
	tasklist listTask_;

	// フラグ
	bool bKill_;
	bool bRemove_;

	virtual void callTaskAction(CTaskContext* pContext);
	virtual void callTaskDraw(CTaskContext* pContext);
};

} // namespace Task end
} // namespace BMW end