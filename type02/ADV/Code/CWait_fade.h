/*
	katze 05/05/24
	フェード待ち
*/
#pragma once

namespace BMW{
namespace ADV{
namespace API{

class CWait_fade : public Task::ITaskList
{/**
	フェード待ち
 */
public:
	// タスク
	void Task(Task::CTaskContext*){}
	void OnInit(Task::CTaskContext*);
	void eventFade(Task::CTaskContext*);
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end