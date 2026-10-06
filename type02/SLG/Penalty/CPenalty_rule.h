/*
	katze 05/07/06
	update 06/02/22
	ペナルティルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Penalty{

class CPenalty_rule : public Task::CTaskList
{/**
	ペナルティルール
 */
public:
	enum eState{
		NORMAL,
		OK,
	};
	enum ePriority{
		OK_T,
		PANEL,
	};
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
};

} // namespace Penalty end
} // namespace SLG end
} // namespace BMW end