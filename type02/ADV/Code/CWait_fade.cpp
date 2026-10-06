#include "stdafx.h"

#include "CWait_fade.h"

namespace BMW{
namespace ADV{
namespace API{

void CWait_fade::OnInit(Task::CTaskContext* pContext)
{
	Scene::CFoward::FaderEvent fun;
	fun.set(this,&CWait_fade::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(fun);
}

void CWait_fade::eventFade(Task::CTaskContext* pContext)
{
	getTaskListCtrl()->returnTaskList();
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end