/*
	katze 07/01/22
	告白ルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Love{

class CLove_rule : public Task::ITaskList
{/**
	告白ルール

	基本的には、告白イベントを呼ぶだけ
	呼んだら終了する
 */
public:
	enum eState{
		NORMAL,
		WAIT,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);
};

} // namespace Love end
} // namespace SLG end
} // namespace BMW end