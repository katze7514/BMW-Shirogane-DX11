/*
	katze 05/04/13
	初期ロードシーン
*/
#pragma once

namespace BMW{
namespace InitLoad{

class CInitLoadScene : public Task::CTaskList
{/**
	初期ロードシーン

	全体共通のデータ類の初期化をする
 */
public:
	enum eState{
		FIRST,
		INIT,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// 実際に初期化をする
	void Init(Task::CTaskContext*);
};

} // namespace InitLoad end
} // namespace BMW end