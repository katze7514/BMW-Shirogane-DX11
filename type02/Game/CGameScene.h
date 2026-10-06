/*
	katze 05/06/13
	ADV・SLG・Interを切り替えるためのシーン
*/
#pragma once

namespace BMW{

namespace Scenario{
class CDataScenario;
} // namespace Scenario end

namespace Game{

class CGameScene : public Task::ITaskList
{/**
	ADV・SLG・Interを切り替えるためのシーン
	今まで、暫定的にタイトルシーンが行っていたこと
 */
public:
	enum eState{
		NORMAL,
		NEXT,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アクション
	void actionNext(Task::CTaskContext*);

private:
	// モード
	int nMode_;
	// 現在実行中のシナリオ
	Scenario::CDataScenario* pScenario_;

	// SLG後の分岐
	void afterSLG(int nID, int nFlag, Task::CTaskContext*);
};

} // namespace Game end
} // namespace BMW end