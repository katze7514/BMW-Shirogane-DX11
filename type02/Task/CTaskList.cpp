#include "stdafx.h"

#include "CTaskContext.h"
#include "CTaskList.h"

namespace BMW{
namespace Task{

void CTaskList::callTaskAction(CTaskContext* pContext)
{// 動作用	
	// 子タスクを呼ぶ前にTaskListを入れ替える
	ITaskList* prevList=pContext->getTaskList();
	pContext->setTaskList(this);

	tasklist::iterator it=listTask_.begin();
	while(it!=listTask_.end())
	{
		(*it)->Task(pContext);
		if(IsKill() || IsRemove()){
			// killフラグだったらdelete
			if(IsKill()){ DELETE_SAFE(*it); }

			it=listTask_.erase(it);

			// フラグリセット
			bKill_=false;
			bRemove_=false;
		}
		else
		{
			++it;
		}
	}

	// 子タスクを呼んだのでTaskListを元に戻す
	pContext->setTaskList(prevList);
}

void CTaskList::callTaskDraw(CTaskContext* pContext)
{// 描画用
 // 描画の時は、killMeなどが呼ばれることが無いので、チェックする必要がない
 // CSurfaceChacheが必要な時はこいつをオーバーライドして、
 // /**～*/ で囲われてる部分を追加する感じにすればOKだと思われる
	/**
		どっかにchacheというCSurfaceChacheがあるとする

		CPlane* prevPlane=pContext->getDrawPlane();
		CPlane plane(smart_ptr<ISurface>(&chache,false));
		pContext->setDrawPlane(&plane);
	*/

	tasklist::iterator it=listTask_.begin();
	while(it!=listTask_.end())
	{/**
		chache.setPriority((*it)->getDrawPriority());
	 */
		(*it)->Task(pContext);
		++it;
	}

	/**
		pContext->setDrawPlane(prePlane);
	*/
}

/////////////////////////////////////////////////////
// タスクリスト操作
/////////////////////////////////////////////////////
void CTaskList::addTask(ITaskBase* pBase, int nPri)
{
	pBase->setTaskPriority(nPri);
	pBase->setParent(smart_ptr<ITaskBase>(this,false));
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it)
		if((*it)->getTaskPriority() > nPri) break;

	listTask_.insert(it,pBase);
}

void CTaskList::addTask(ITaskBase* pBase)
{
	pBase->setTaskPriority(listTask_.size());
	pBase->setParent(smart_ptr<ITaskBase>(this,false));
	listTask_.push_back(pBase);
}

void CTaskList::killTask(int nPri)
{
	tasklist::iterator it=listTask_.begin();
	while(it!=listTask_.end())
	{
		if((*it)->getTaskPriority() > nPri) break;
		if((*it)->getTaskPriority() == nPri)
		{
			DELETE_SAFE(*it);
			it=listTask_.erase(it);
		}
		else
		{ ++it; }
	}
}

ITaskBase* CTaskList::removeTask(int nPri)
{
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it){
		if((*it)->getTaskPriority() > nPri) break;
		if((*it)->getTaskPriority() == nPri){
			ITaskBase* p= *it;
			listTask_.erase(it);
			return p;
		}
	}
	return NULL;
}

void CTaskList::clearTask()
{
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it) DELETE_SAFE(*it); 
	//for_each(listTask_.begin(),listTask_.end(),Delete());
	listTask_.clear();
}

ITaskBase* CTaskList::getTask(int nPri)
{
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it){
		if((*it)->getTaskPriority() > nPri)  break;
		if((*it)->getTaskPriority() == nPri) return *it;
	}
	return NULL;
}

void CTaskList::callTaskReset(Task::CTaskContext* pContext)
{
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it)
		(*it)->OnReset(pContext);
}

} // namespace Task end
} // namespace BMW end