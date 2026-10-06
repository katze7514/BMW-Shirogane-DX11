/*
	katze 05/06/25
	Inter用TaskListCtrl
*/
#pragma once

namespace BMW{
namespace Inter{

class CInterTaskListCtrl : public Task::CTaskListCtrlChache
{/**
	Inter用TaskListCtrl
 */
public:
	// デストラクタ
	virtual ~CInterTaskListCtrl(){}

	void OnReset(Task::CTaskContext*);

protected:
	void noExist(Task::CTaskContext*);
};

} // namespace Inter end
} // namespace BMW end