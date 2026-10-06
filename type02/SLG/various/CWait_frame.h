/*
	katze 06/07/22
	ウェイト
*/
#pragma once

namespace BMW{
namespace SLG{

class CWait_frame : public Task::ITaskList
{/**
	ウェイト
 */
public:
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
};

} // namespace SLG end
} // namespace BMW end