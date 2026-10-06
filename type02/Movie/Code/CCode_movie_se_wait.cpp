#include "stdafx.h"

#include "CCode_movie_se_wait.h"

namespace BMW{
namespace Movie{
namespace Code{

void CCode_se_wait::OnAction(Task::CTaskContext* pContext)
{// stopフレームと組み合わせて使う
	if(pContext->getApp()->getSeDB().IsPlay(getState()))
	{// 再生中だったら、スクリプトを進めない
		pContext->getTaskList()->removeMe();
	}
	else
	{// 止まったら
		// フレームを進める
		pContext->getTaskList()->getParent()->setState(1);
	}
	// 状況は進める
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace Movie end
} // namespace BMW end