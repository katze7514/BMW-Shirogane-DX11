#include "stdafx.h"

#include "CWait_input.h"

namespace BMW{
namespace ADV{
namespace API{

void CWait_input::OnInit(Task::CTaskContext* pContext)
{
	pContext->getInput()->guard(false);
#ifdef BMW_DEBUG
	nFrame_=0;
#endif
}

#ifdef BMW_DEBUG
static int WAIT_FRAME=10;
#endif

void CWait_input::OnAction(Task::CTaskContext* pContext)
{
	// OKボタンリリース・CRTLが押されてたら、すぐに次へ
	Input::IInput* pInput = pContext->getInput();
	if(pInput->getInputState(Input::IInput::OK)==Input::IInput::RELEASE
	|| pInput->getInputState(Input::IInput::CTRL)!=Input::IInput::NO)
	{
		pContext->getInput()->guard(true);
		getTaskListCtrl()->returnTaskList();
	}
	else if(pInput->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE
			&& getState()>=0)
	{// キャンセルが押されたらバックログモード？
		pContext->getInput()->guard(true);
		getTaskListCtrl()->callTaskList(getState(),true);
	}
#ifdef BMW_DEBUG
	// デバッグ時はオートリードもｗ
	//if(++nFrame_>WAIT_FRAME)
	//{
	//	pContext->getInput()->guard(true);
	//	getTaskListCtrl()->returnTaskList();
	//}
#endif
}

void CWait_input::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	pContext->getInput()->guard(false);
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end