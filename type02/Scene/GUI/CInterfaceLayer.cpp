#include "stdafx.h"

#include "../../Movie/CKeyFrame.h"

#include "CInterfaceLayer.h"

namespace BMW{
namespace GUI{

CInterfaceLayer::~CInterfaceLayer()
{
	frame_vec::iterator it;
	for(it=vecFrame_.begin(); it!=vecFrame_.end(); it++)
		DELETE_SAFE(*it);

	vecFrame_.clear();

	DELETE_SAFE(pPanel_);
}


void CInterfaceLayer::Task(Task::CTaskContext* pContext)
{
	// キーフレーム動作
	// このレイヤーは、フレーム切り替えを行わない
	if(pContext->IsAction())
	{
		if(IsValid()) vecFrame_[getState()]->Task(pContext);
	}
	else
	{
		if(IsVisible()) vecFrame_[getState()]->Task(pContext);
	}
}

void CInterfaceLayer::OnReset(Task::CTaskContext* pContext)
{
	frame_vec::iterator it;
	for(it=vecFrame_.begin(); it!=vecFrame_.end(); it++)
		(*it)->OnReset(pContext);

	setState(0);
}


void CInterfaceLayer::setKeyFrame(Movie::IKeyFrame* pFrame, int nState)
{
	#ifdef BMW_DEBUG
		if(pPanel_==NULL)
		{
			CDbg().Out("InterfaceLayerを設定するまえに対象を設定してください");
			return;
		}
	#endif
	// 親
	pFrame->setParent(smart_ptr<Task::ITaskBase>(this,false));
	// パネルを設定しておく
	pFrame->setTask(pFrame);
	// 共有フラグはON
	pFrame->share(true);
	// フレームvectorへ
	vecFrame_[nState]=pFrame;
}

void CInterfaceLayer::resetKeyFrame(int nState,Task::CTaskContext* pContext)
{
	vecFrame_[nState]->OnReset(pContext);
}

bool CInterfaceLayer::IsEnd()
{
	return vecFrame_[getState()]->IsEnd();
}

} // namespace GUI end
} // namespace BMW edn