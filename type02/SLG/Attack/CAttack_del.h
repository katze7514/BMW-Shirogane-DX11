/*
	katze 05/07/20
	戦闘結果に合わせて、死亡したキャラを
	マップから排除する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CAttack_del : public Task::ITaskList
{/**
	戦闘結果に合わせて、死亡したキャラを
	マップから排除する
 */
public:
	enum eState{
		NORMAL,
		DEL,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

private:
	// 死亡するデータリスト
	list<int> listDeath_;
	list<int>::iterator it;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end