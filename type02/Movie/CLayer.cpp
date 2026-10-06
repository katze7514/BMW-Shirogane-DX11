#include "stdafx.h"

#include "IKeyFrame.h"

#include "CLayer.h"

namespace BMW{
namespace Movie{

void CLayer::Task(Task::CTaskContext* pContext)
{
	if(getState()>=0)
	{
		// 実行キーフレームの更新
		nExec_=getState();
		// キーフレーム動作
		vecKeyFrame_[nExec_]->Task(pContext);

		// 切り替わり判定
		if(pContext->IsAction()) OnAction(pContext);
	}
}

void CLayer::OnAction(Task::CTaskContext* pContext)
{// キーフレームの切り替え
	int nChange = vecKeyFrame_[nExec_]->getState();
	// 切り替わりフレーム数が負の場合は切り替わらない
	if(nChange<0) return;
	if(nFrame_>=nChange)
	{// 切り替わりフレームを越えていたら、次のキーフレームへ
		++nState_;
		nFrame_=1;

		if(getState()>=(int)vecKeyFrame_.size())
		{// キーフレームが全て消化された 
			setState(END);
			pContext->getTaskList()->killMe();
		}
		else
		{// フレーム要素共有？
			if(vecKeyFrame_[nState_]->IsShare())
				vecKeyFrame_[nState_]->setTask(vecKeyFrame_[nExec_]->getTask());
		}
	}
	else
	{ ++nFrame_; }
}

void CLayer::OnReset(Task::CTaskContext* pContext)
{
	frame_vec::iterator it;
	for(it=vecKeyFrame_.begin(); it!=vecKeyFrame_.end(); it++)
		(*it)->OnReset(pContext);

	setState(0);
	nExec_=0;
	nFrame_=1;
}
void CLayer::setKeyFrame(IKeyFrame* pKeyFrame, int nKey)
{
	// こうすることで、KeyFrameの要素にCMovieClipが親にできる
	pKeyFrame->setParent(getParent());
	pKeyFrame->setTaskPriority(nKey);
	vecKeyFrame_[nKey]=pKeyFrame;
}

void CLayer::clearKeyFrame()
{
	frame_vec::iterator it;
	for(it=vecKeyFrame_.begin(); it!=vecKeyFrame_.end(); it++)
		DELETE_SAFE(*it);
	//for_each(vecKeyFrame_.begin(),vecKeyFrame_.end(),Delete());

	vecKeyFrame_.clear();
}

void CLayer::getSize(LONG& lWidth, LONG& lHeight) const
{
	if(getState()>=0)
		vecKeyFrame_[nExec_]->getSize(lWidth,lHeight);
	else
		vecKeyFrame_[0]->getSize(lWidth,lHeight);
}

} // namespace Movie end
} // namespace BMW end