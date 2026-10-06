/*
	katze 06/06/11
	マップ武器ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CField_rule : public Task::ITaskList
{/**
	マップ武器ルール
 */
public:
	enum eState
	{// 状態
		SELECT,
		ATTACK,
		BATTLE_START,
		EFFECT,
		DEMO,
		APPLY,
		BATTLE_END,
		DEL,
		RESULT,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end