/*
	katze 05/07/10
	セーブデータへの反映
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Data{

class CData_ref : public Task::ITaskList
{/**
	セーブデータへの反映
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Data end
} // namespace SLG end
} // namespace BMW end