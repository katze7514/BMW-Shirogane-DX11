/*
	katze 06/04/19
	戦闘イベントAPI
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataBattle;
class CSLGContext;

namespace Event{
class CBattleEventData;

class CEvent_Battle2 : public Task::ITaskList
{/**
	戦闘イベントのための補助クラス

	戦闘イベントを起こす時は、こいつを継承して
	おくと便利やぞ
 */
public:
	enum eState{
		NORMAL,
		DEMO,
		APPLY,
		END,
	};
	// デストラクタ
	virtual ~CEvent_Battle2(){}
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

private:
	// デモフラグ保存のため
	int nDemo_;
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end