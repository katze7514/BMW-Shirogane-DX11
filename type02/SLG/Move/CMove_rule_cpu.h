/*
	katze 05/05/17
	移動ルール for CPU
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Move{

class CMove_rule_cpu : public Task::ITaskList
{/**
	移動ルール for CPU	
 */
public:
	enum eState{
		VIEW,
		ROAD,
		EXEC,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext* pContext);
	void OnAction(Task::CTaskContext* pContext);
	void OnComeBack(int nID,Task::CTaskContext* pContext);

private:
	int nWait_;
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end