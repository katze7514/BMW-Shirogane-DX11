/*
	katze 05/07/01
	精神ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Spirit{

class CSpirit_rule : public Task::ITaskList
{/**
	精神ルール
 */
public:
	enum eState{
		MENU,
		SELECT,
		EFFECT,
		APPLY,
		END,
	};

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID,Task::CTaskContext*);
};

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end