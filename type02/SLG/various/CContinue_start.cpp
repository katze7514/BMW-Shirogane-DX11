#include "stdafx.h"

#include "../CSLGScene.h"
#include "../IDRule.h"
#include "../IDSLG.h"

#include "../VM/CSlgVM.h"

#include "CContinue_start.h"

namespace BMW{
namespace SLG{

void CContinue_start::OnInit(Task::CTaskContext* pContext)
{
	// コンテニューだ！
	setState(NORMAL);
}

void CContinue_start::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
		callScript("CONTINUE_START_EVENT", pContext);
	break;

	case END:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		p->getScene()->setState(CSLGScene::NORMAL);
		getTaskListCtrl()->jumpTaskList(Rule::MAIN);
	}
	break;

	default: break;
	}
}

void CContinue_start::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Scene::CFoward::FaderEvent fun(this, &CContinue_start::eventFade);
	// フェードアウト
	pContext->getApp()->getFoward()->setFaderHandler(fun);
	pContext->getApp()->getFoward()->fadeOut();
	if(p->getBgm()>=0)
	{// BGMが鳴ってたらそれをならす
		pContext->getBgmSound()->change(p->getBgm());
		pContext->getBgmSound()->Play();
	}

	setState(FADE);
}

void CContinue_start::eventFade(Task::CTaskContext* pContext)
{
	setState(END);
}

} // namespace SLG end
} // namespace BMW end