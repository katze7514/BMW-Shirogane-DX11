#include "stdafx.h"

#include "IDDemo.h"
#include "CDemoMovieClip.h"
#include "CLayerChara.h"

namespace BMW{
namespace Demo{

void CLayerChara::Task(Task::CTaskContext* pContext)
{
	vecTask_[getState()]->Task(pContext);

	if(!IsEnd())
	{
		if(pContext->IsAction())
			OnAction(pContext);
	}
}

void CLayerChara::OnAction(Task::CTaskContext* pContext)
{
	// 実行中のムービーの終了判定
	if(vecTask_[getState()]->getState()==CDemoMovieClip::END)
	{
		if(++nState_==(int)vecTask_.size())
		{// 終了
			bEnd_=true;
			// フィールド武器時は、最後のムービーで止まって終了する
			if(pContext->getValue(Flag::FIELD)){ --nState_; }
			// そうじゃなかったら、何もしないタスクをツッコンでしまう
			// こうしておけば、実行イテレータが全体より一つ進んでいても大丈夫
			else addTask(new ITaskBase());
		}
	}
}

void CLayerChara::OnReset(Task::CTaskContext* pContext)
{
	task_vec::iterator it;
	for(it=vecTask_.begin(); it!=vecTask_.end(); ++it)
		(*it)->OnReset(pContext);

	setState(0);
}

void CLayerChara::addTask(Task::ITaskBase* pBase)
{
	pBase->setTaskPriority((int)vecTask_.size());
	vecTask_.push_back(pBase);
}

void CLayerChara::clearTask()
{
	task_vec::iterator it;
	for(it=vecTask_.begin(); it!=vecTask_.end(); ++it)
		DELETE_SAFE(*it);

	vecTask_.clear();
}

} // namespace Demo end
} // namespace BMW end