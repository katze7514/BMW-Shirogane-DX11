#include "stdafx.h"

#include "../CApp.h"
#include "CTaskContext.h"
#include "ITaskList.h"
#include "ITaskListFactory.h"

#include "CTaskListCtrlChache.h"

namespace BMW{
namespace Task{

void CTaskListCtrlChache::OnAction(CTaskContext* pContext)
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
		// 現在のシーンの削除フラグを立てる
		vecList_.rbegin()->bDelete_=true;
		createTaskList(nNext,pContext);
	break;

	case MES_CALL_FAST:
		// 現在のシーンの削除フラグを倒す
		vecList_.rbegin()->bDelete_=false;
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
		CTaskListChacheInfo& pInfo = *vecList_.rbegin();
		// 削除フラグが立っているかどうか
		if(pInfo.bDelete_)
		{// OnResetを呼び出す
			int nScene=pInfo.nTaskListID_;
			vecList_.pop_back();
			createTaskList(nScene,pContext);
		}
		else
		{// 現在、スタックトップのをカレントタスクリストにする
			map<int, smart_ptr<ITaskList> >::iterator it = mapChache_.find(pInfo.nTaskListID_);
			pCurrentTaskList_=it->second;
		}
		// カムバ～ク
		getCurrentTaskList()->OnComeBack(nReturn,pContext);
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

void CTaskListCtrlChache::OnDraw(CTaskContext* pContext)
{
	CurrentTask(pContext);
}

void CTaskListCtrlChache::CurrentTask(CTaskContext* pContext)
{// 現在のTaskListのタスク処理
	if(!getCurrentTaskList().isNull())
		pCurrentTaskList_->Task(pContext);
}

void CTaskListCtrlChache::createTaskList(int nID,CTaskContext* pContext)
{// TaskListを生成してスタックに積む
	CTaskListChacheInfo info;
	info.nTaskListID_=nID;
	
	// キャッシュに存在するかをチェック
	map<int, smart_ptr<ITaskList> >::iterator it;
	it=mapChache_.find(nID);
	if(it!=mapChache_.end())
	{// 在ったらOnResetを呼び出す
		(it->second)->OnReset(pContext);
		pCurrentTaskList_=it->second;
	}
	else
	{// 無かったらキャッシュ生成
		smart_ptr<ITaskList> pList = createChache(nID,pContext);
		// 生成したTaskListを現在のTaskListとして設定
		pList->OnReset(pContext);
		pCurrentTaskList_=pList;
	}
	// スタックに積む
	vecList_.push_back(info);
}

smart_ptr<ITaskList> CTaskListCtrlChache::createChache(int nID, CTaskContext* pContext)
{
	smart_ptr<ITaskList> pList = getTaskListFactory()->createTaskList(nID);
	if(!pList.isNull())
	{
		pList->setTaskListCtrl(smart_ptr<ITaskListCtrl>(this,false));
		// こいつが管理するリストの親は、このCtrlの親と等しい
		pList->setParent(getParent());
		pList->OnInit(pContext);
		// キャッシュに設定
		mapChache_.insert(pair<int,smart_ptr<ITaskList> >(nID, pList));
	}
	return pList;	
}

void CTaskListCtrlChache::noExist(CTaskContext* pContext)
{
	// ゲーム終了～
	pContext->getApp()->end();
}

} // namespace Task end
} // namespace BMW end