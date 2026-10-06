/*
	katze 05/05/18
	入力待ち
*/
#pragma once

namespace BMW{
namespace ADV{
namespace API{

class CWait_input : public Task::ITaskList
{/**
	入力待ち
 */
public:
	// コンストラクタ
	CWait_input(){ setState(-1); }
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

#ifdef BMW_DEBUG
	int nFrame_;
#endif
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end