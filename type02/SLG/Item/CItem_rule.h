/*
	katze 05/07/01
	アイテムルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Item{

class CItem_rule : public Task::ITaskList
{/**
	精神ルール
 */
public:
	enum eState{
		MENU,
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

} // namespace Item end
} // namespace SLG end
} // namespace BMW end