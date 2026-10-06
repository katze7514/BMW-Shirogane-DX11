/*
	katze 05/07/09
	補給ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Refill{

class CRefill_rule : public Task::ITaskList
{/**
	補給ルール

	基本的には、攻撃ルールと同じ
	データ設定も、CDataBattleを流用する
 */
public:
	enum eState{
		RANGE,
		SELECT,
		DEMO,
		APPLY,
		RESULT,
		WAIT,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);
};

} // namespace Refill end
} // namespace SLG end
} // namespace BMW end