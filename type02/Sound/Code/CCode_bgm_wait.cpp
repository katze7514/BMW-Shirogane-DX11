#include "stdafx.h"

#include "CCode_bgm_wait.h"

namespace BMW{
namespace Sound{
namespace Code{

void CCode_bgm_wait::OnAction(Task::CTaskContext* pContext)
{// こいつはサブルーチン扱いなので注意
	// フェードが終了してたらリターンする
	if(pContext->getBgmSound()->getState()==Sound::Ctrl::PLAY
	|| pContext->getBgmSound()->getState()==Sound::Ctrl::STOP)
		getTaskListCtrl()->returnTaskList();
}

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
