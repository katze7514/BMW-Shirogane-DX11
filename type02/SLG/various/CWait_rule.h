/*
	katze 05/05/17
	待機ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Wait{

class CWait_rule : public Task::ITaskList
{/**
	待機ルール

	ようは、操作対象キャラをAFTERにする
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Wait end
} // namespace SLG end
} // namespace BMW end