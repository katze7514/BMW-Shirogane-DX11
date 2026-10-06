#include "stdafx.h"

#include "CInterScene.h"
#include "CInterTaskListCtrl.h"

namespace BMW{
namespace Inter{

void CInterTaskListCtrl::OnReset(Task::CTaskContext* pContext)
{// キャッシュ内タスクリストのOnResetを呼ぶ
	map<int, smart_ptr<Task::ITaskList> >::iterator it;
	for(it=mapChache_.begin(); it!=mapChache_.end(); it++)
		(it->second)->OnReset(pContext);
}

void CInterTaskListCtrl::noExist(Task::CTaskContext* pContext)
{
	getParent()->setState(CInterScene::END);
}

} // namespace Inter end
} // namespace BMW end