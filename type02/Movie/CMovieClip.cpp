#include "stdafx.h"

#include "CLayer.h"
#include "CMovieClip.h"

namespace BMW{
namespace Movie{

void CMovieClip::OnAction(Task::CTaskContext* pContext)
{
	// デフォルトでループ
	if(IsEnd()) OnReset(pContext);
}

void CMovieClip::callTaskAction(Task::CTaskContext* pContext)
{// 子タスクを呼ぶ前にTaskListを入れ替える
	ITaskList* prevList=pContext->getTaskList();
	pContext->setTaskList(this);

	tasklist::iterator it=listTask_.begin();
	while(it!=listTask_.end())
	{
		(*it)->Task(pContext);
		if(IsKill()){
			// キルフラグが立っているなら、
			// Layerのキーフレームが全て消化された
			++nEndLayer_;
			// フラグリセット
			bKill_=false;
		}
		++it;
	}

	// 子タスクを呼んだのでTaskListを元に戻す
	pContext->setTaskList(prevList);
}

void CMovieClip::OnReset(Task::CTaskContext* pContext)
{
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); it++)
		(*it)->OnReset(pContext);

	nEndLayer_=0;
	setState(NORMAL);
}

bool CMovieClip::IsEnd()
{
	if(nEndLayer_>=(int)listTask_.size()) setState(END);
	return getState()==END;
}

bool CMovieClip::IsStop()
{
	return getState()==STOP || !IsValid();
}

void CMovieClip::getSize(LONG& lWidth, LONG& lHeight)
{
	lWidth=lHeight=0;
	LONG Width,Height;
	tasklist::iterator it;
	for(it=listTask_.begin(); it!=listTask_.end(); ++it)
	{
		(*it)->getSize(Width,Height);
		if(lWidth<Width) lWidth=Width;
		if(lHeight<Height) lHeight=Height;
	}
}

} // namespace Movie end
} // namespace BMW end