/*
	katze 05/05/17
	攻撃ルール for CPU
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CAttack_rule_cpu : public Task::ITaskList
{/**
	攻撃ルール for CPU
 */
public:
	enum eState{
		RANGE,
		VIEW, // 攻撃カーソル移動も行う
		FIELD_RULE,
		ACTION,
		BATTLE_START,
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

private:
	int nWait_;
	bool bMap_;
};

} // namespace Attack end
} // namesapce SLG end
} // namespace BMW end