/*
	katze 05/05/19
	背景変更
*/
#pragma once

namespace BMW{
namespace ADV{
namespace API{

class CBack_change : public Task::ITaskList
{/**
	背景変更

	変更する背景IDをスタックに積んでおく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end