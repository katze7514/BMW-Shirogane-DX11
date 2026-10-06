#include "stdafx.h"

#include "CCode_se_wait.h"

namespace BMW{
namespace Sound{
namespace Code{

void CCode_se_wait::OnInit(Task::CTaskContext* pContext)
{// スタックトップに再生待ちSE IDが積まれている
	nID_=pContext->top();
	pContext->pop();

	bInput_ = pContext->getInput()->IsGuard();
	pContext->getInput()->guard(false);
}

void CCode_se_wait::OnAction(Task::CTaskContext* pContext)
{
	if(!pContext->getApp()->getSeDB().IsPlay(nID_))
	{// 再生が終了してたらリターンする
		pContext->getInput()->guard(bInput_);
		getTaskListCtrl()->returnTaskList();
	}
	ef(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::CANCEL
	|| pContext->getInput()->getInputState(Input::IInput::CTRL)!=Input::IInput::NO)
	{// キャンセルされたら、音を止める（ホントはFadeOutの方が良いかも・・・）
		pContext->getApp()->getSeDB().Stop(nID_);
		pContext->getInput()->guard(true);
	}

}

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
