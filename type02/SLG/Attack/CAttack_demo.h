/*
	katze 05/05/07
	戦闘デモ
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataBattleBase;

namespace Attack{

class CAttack_demo : public Task::ITaskList
{/**
	戦闘デモ

	のディスパッチャでもある
 */
public:
	enum eState{
		DEMO,		// デモシーンへ
		DEMO_MAP,	// マップ上デモ
		DEMO_END,	// デモ終了
		END,		// 終了
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// 操作
	void playBgm(CDataBattleBase& player, CDataBattleBase& enemy, int nSide, Task::CTaskContext* pContext);
	void playBgmEvent(CDataBattleBase& attack, CDataBattleBase& counter, Task::CTaskContext* pContext);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end