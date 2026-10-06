#include "stdafx.h"

#include "CPanel.h"

namespace BMW{
namespace GUI{

void CPanelList::addTask(ITaskBase* pBase, int nPri)
{
	pBase->setTaskPriority(nPri);
	pBase->setParent(getParent());
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); it++)
		if((*it)->getTaskPriority() > nPri) break;

	listTask_.insert(it,pBase);
}

//////////////////////////////////////////////////////////////////
CPanel::CPanel()
{
	listWidget_.setParent(smart_ptr<Task::ITaskBase>(this,false));
}

void CPanel::Task(Task::CTaskContext* pContext)
{
	listWidget_.Task(pContext);
	if(pContext->IsAction()
	&& IsValid())
		OnAction(pContext);
}

void CPanel::OnReset(Task::CTaskContext* pContext)
{
	listWidget_.callTaskReset(pContext);
}

void CPanel::addWidget(Task::ITaskBase* pBase, const string& sID)
{
	setID(sID);
	listWidget_.addTask(pBase, getID(sID));
}

Task::ITaskBase* CPanel::swapWidget(Task::ITaskBase* pBase, const string& sID, bool bDelete)
{
	Task::ITaskBase* pOld = listWidget_.removeTask(getID(sID));
	if(pOld!=NULL)
	{// 正しく取得できたら
		// これをつかうときは位置情報だけは欲しいのでコピー
		pBase->setDrawInfo(pOld->getDrawInfo(false));
		// フラグに合わせて、deleteをかけたりかけなかったり
		if(bDelete) DELETE_SAFE(pOld);
	}

	listWidget_.addTask(pBase, getID(sID));
	// デリートされてないなら、旧データを返す
	if(bDelete) return NULL;
	else		return pOld;
}

void CPanel::validAll(bool bV)
{
	CPanelList::tasklist& listTask = listWidget_.getTaskList();
	CPanelList::tasklist::iterator it;
	for(it=listTask.begin(); it!=listTask.end(); it++)
		(*it)->valid(bV);
}

void CPanel::visibleAll(bool bV)
{
	CPanelList::tasklist& listTask = listWidget_.getTaskList();
	CPanelList::tasklist::iterator it;
	for(it=listTask.begin(); it!=listTask.end(); it++)
		(*it)->visible(bV);
}

} // namespace GUI end
} // namespace BMW end