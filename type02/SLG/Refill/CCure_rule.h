/*
	katze 05/05/30
	治療ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Cure{

class CCure_rule : public Task::ITaskList
{/**
	治療ルール

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

	// 治療計算
	void calcCure(Task::CTaskContext*);
};

} // namespace Cure end
} // namespace SLG end
} // namespace BMW end