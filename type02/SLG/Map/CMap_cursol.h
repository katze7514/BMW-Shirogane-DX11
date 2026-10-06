/*
	katze 05/07/29
	マップカーソル誘導
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Map{

class CMap_cursol : public Task::ITaskList
{/**
	マップカーソル誘導
 */
public:
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	Movie::CMotion motion_;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end