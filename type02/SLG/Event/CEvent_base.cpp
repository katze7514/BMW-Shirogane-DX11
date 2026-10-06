#include "stdafx.h"

#include "../IDSLG.h"
#include "../CSLGScene.h"
#include "../Context/CSLGContext.h"
#include "CEvent_base.h"

namespace BMW{
namespace SLG{

void CEvent_base::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
#ifdef BMW_DEBUG
		CDbg().Out(sEventName_.c_str());
#endif
		pContext->getInput()->cursolVisible(false);
		getTaskListCtrl()->callTaskList(getID(),true);
	break;


	case END:
		// ログのクリア
		static_cast<CSLGContext*>(pContext)->clearBackLog();

		// EVENT内でSLG_ENDが呼ばれてるかもしれない
		if(pContext->getScene()->getState()==CSLGScene::END) 
		{// そしたらカーソルは表示せず
			pContext->getInput()->cursolVisible(false);
			// リターンしないで処理終了を待つ
			setState(WAIT);
		}
		else
		{
			pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
			// そしてリターン
			getTaskListCtrl()->returnTaskList();
		}
	break;

	default: break;
	}
}

void CEvent_base::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	setState(END);
}

} // namespace SLG end
} // namespace BMW end