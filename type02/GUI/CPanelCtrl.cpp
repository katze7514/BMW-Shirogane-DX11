#include "stdafx.h"

#include "CPanelCtrl.h"

namespace BMW{
namespace GUI{

CPanelCtrl::CPanelCtrl()
{
	ctrlWidget_.setParent(smart_ptr<Task::ITaskBase>(this,false));
	// NULL DEVICE
	ctrlWidget_.addTask(new Task::ITaskBase(),-1);
}

/////////////////////////////////////////////
// タスク
/////////////////////////////////////////////
void CPanelCtrl::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			ctrlWidget_.Task(pContext);
			OnAction(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			ctrlWidget_.Task(pContext);
		}
	}
}

void CPanelCtrl::OnReset(Task::CTaskContext* pContext)
{
	ctrlWidget_.OnReset(pContext);
}
//////////////////////////////////////////////
// 管理
//////////////////////////////////////////////
void CPanelCtrl::validWidget(int nID)
{
	// こっちだとNullDeviceが発動しないかも
	// しれないから注意
	ctrlWidget_.setState(nID);
}

void CPanelCtrl::validWidget(const string& sID)
{
	// 無効なIDが来てもNullDeviceがあるので大丈夫
	ctrlWidget_.setState(getID(sID));
}

//////////////////////////////////////////////
// 設定とか
//////////////////////////////////////////////
void CPanelCtrl::addWidget(Task::ITaskBase* pBase, const string& sID)
{
	setID(sID);
	ctrlWidget_.addTask(pBase,getID(sID));
}

//template<class T>
//T* CPanelCtrl::getWidgetCast(const string& sID)


} // namespace GUI end
} // namespace BMW end