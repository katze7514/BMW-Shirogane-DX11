/*
	katze 06/06/09
	攻撃ルールVer.2
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CAttack_rule2 : public Task::ITaskList
{/**
	attacl rule
	攻撃ルール
 */
public:
	enum eState{
		WEAPON,
		FIELD,
		SELECT,
		CALC,
		ACTION,
		BATTLE_START,
		DEMO,
		APPLY,
		BATTLE_END,
		DEL,
		RESULT,
		HIT_AWAY,
		WAIT,
		CANCEL,
		END,
		WAIT_END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アクション
	void attackCalcCancel(Task::CTaskContext*);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end