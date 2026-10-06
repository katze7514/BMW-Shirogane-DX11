/*
	katze 05/04/26
	メニューセレクト for Player
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Menu{

class CMenu_select_player : public BMW::Rule::CRuleList
{/**
	メニューセレクト for Player
 */
public:
	enum eState{
		NORMAL,
		CHARA,
		TURN,
		STATUS,
		CHARA_END,
		VICTORY,
		VICTORY_CHANGE,
		END,
	};
	enum ePriority{
		OK,
		CANCEL,
		NO,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	void callRule(int nID, Task::CTaskContext* pContext);
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end