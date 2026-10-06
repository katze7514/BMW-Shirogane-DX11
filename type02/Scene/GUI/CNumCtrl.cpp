#include "stdafx.h"

#include "CNumCtrl.h"

namespace BMW{
namespace GUI{

CNumCtrl::CNumCtrl()
{
	ctrlNum_.setParent(smart_ptr<Task::ITaskBase>(this,false));
	ctrlNum_.addTask(new INum(), -1);
}


/////////////////////////////////////////////////////////////
// タスク
/////////////////////////////////////////////////////////////
void CNumCtrl::Task(Task::CTaskContext* pContext)
{
	ctrlNum_.Task(pContext);
}

//////////////////////////////////////////////////////////////
// 設定・取得
/////////////////////////////////////////////////////////////
LONG CNumCtrl::getNum() const
{
	num_map::const_iterator it = getNumMap().find(getState());
	if(it==getNumMap().end()) return 0;
	return (it->second)->getNum();
}

void CNumCtrl::setNum(LONG lNum)
{
	num_map::iterator it;
	for(it=getNumMap().begin(); it!=getNumMap().end(); it++)
		(it->second)->setNum(lNum);
}

bool CNumCtrl::IsPlus() const
{
	num_map::const_iterator it = getNumMap().find(getState());
	if(it==getNumMap().end()) return false;
	return (it->second)->IsPlus();
}

void CNumCtrl::plus(bool bPlus)
{
	num_map::iterator it;
	for(it=getNumMap().begin(); it!=getNumMap().end(); it++)
		(it->second)->plus(bPlus);
}

} // namespace GUI end
} // namespace BMW end