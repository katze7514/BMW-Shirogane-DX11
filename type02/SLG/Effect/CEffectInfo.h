/*
	katze 06/05/23
	エフェクトスクリプト管理用構造体
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CEffectInfo
{
public:
	Task::ITaskBase* pEffect_;	// エフェクト実体
	Task::ITaskList* pList_;	// ↑が投入されているタスクリスト

	CEffectInfo(Task::ITaskBase* pEffect=NULL, Task::ITaskList* pList=NULL):pEffect_(pEffect),pList_(pList){}
	~CEffectInfo()
	{
		if(pList_!=NULL) pList_->removeTask(pEffect_->getTaskPriority());
		DELETE_SAFE(pEffect_);
	}
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end