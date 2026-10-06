#include "stdafx.h"

#include "../CApp.h"
#include "CTaskContext.h"
#include "CTaskList.h"
#include "CTaskListCtrl.h"
#include "ITaskListFactory.h"

namespace BMW{
namespace Task{

void CTaskListCtrl::OnAction(CTaskContext* pContext)
{// TaskList操作
	int nMes,nNext;

	do{
	nMes=getState();
	setState(ITaskListCtrl::MES_NO);
	nNext=getNextTaskList();

	switch(nMes)
	{
//	case ITaskListCtrl::MES_NO: break;

	case MES_CALL:
		// 現在のシーンを削除
		if(!IsEnd()){ (*vecList_.rbegin())->pTaskList_.Delete(); }
		createTaskList(nNext,pContext);
	break;

	case MES_CALL_FAST:
		// ただ、積むだけ
		createTaskList(nNext,pContext);
	break;

	case MES_RETURN:
	{
		// 現在のシーンを破棄
		int nReturn=getCurrentTaskListID();
		vecList_.pop_back();

		// 戻るTaskListが無い！？
		if(IsEnd()){ noExist(pContext); break; }

		// 戻るよん
		CTaskListInfo& pInfo = **vecList_.rbegin();
		if(pInfo.pTaskList_.isNull())
		{// factoryから生成～
			int nScene=pInfo.nTaskListID_;
			vecList_.pop_back();
			createTaskList(nScene,pContext);
			if(!((*vecList_.rbegin())->pTaskList_.isNull()))
				(*vecList_.rbegin())->pTaskList_->OnComeBack(nReturn,pContext);
		}
		else
		{	
			pInfo.pTaskList_->OnComeBack(nReturn,pContext);
		}
	}
	break;

	case MES_JUMP:
		if(!IsEnd())
		{// 現在のTaskListを破棄
			vecList_.pop_back();
		}
		createTaskList(nNext,pContext);
	break;

	case MES_EXIT:
		noExist(pContext);
	break;

	default: break;
	}

	CurrentTask(pContext);

	}while(getState()!=ITaskListCtrl::MES_NO);
}

void CTaskListCtrl::OnDraw(CTaskContext* pContext)
{
	CurrentTask(pContext);
}

void CTaskListCtrl::CurrentTask(CTaskContext* pContext)
{// 現在のTaskListのタスク処理
	if(!IsEnd()&&!(*vecList_.rbegin())->pTaskList_.isNull())
		(*vecList_.rbegin())->pTaskList_->Task(pContext);
}

void CTaskListCtrl::createTaskList(int nID,CTaskContext* pContext)
{// TaskListを生成してスタックに積む
	CTaskListInfo* pInfo=new CTaskListInfo();
	pInfo->nTaskListID_=nID;
	pInfo->pTaskList_=getTaskListFactory()->createTaskList(nID);
	if(!pInfo->pTaskList_.isNull())
	{
		pInfo->pTaskList_->setTaskListCtrl(smart_ptr<ITaskListCtrl>(this,false));
		// こいつが管理するリストの親は、このCtrlの親と等しい
		pInfo->pTaskList_->setParent(getParent());
		pInfo->pTaskList_->OnInit(pContext);
	}
	vecList_.push_back(smart_ptr<CTaskListInfo>(pInfo));
}

int CTaskListCtrl::getCurrentTaskListID()
{
	if(IsEnd()) return -1;
	return (*vecList_.rbegin())->nTaskListID_;
}

smart_ptr<ITaskList>& CTaskListCtrl::getCurrentTaskList()
{
	return (*vecList_.rbegin())->pTaskList_;
}

void CTaskListCtrl::noExist(CTaskContext* pContext)
{
	// ゲーム終了～
	pContext->getApp()->end();
}

} // namespace Task end
} // namespace BMW end