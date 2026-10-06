/*
	katze 05/07/09
	アイテム適用
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Item{

class CItem_apply : public Task::ITaskList
{/**
	精神適用
 */
public:
	// タスク
	void OnInit(Task::CTaskContext*);
};

} // namespace Item end
} // namespace SLG end
} // namespace BMW end