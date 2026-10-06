/*
	katze 05/04/26
	update 06/02/23
	ターンメニュー
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Menu{

class CMenu_turn : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
{/**
	ターンメニュー
 */
public:
	enum eState{
		NORMAL,
		CANCEL,
		CALL,
		PHASE_S,
		CANCEL_S,
		CALL_S,
	};
	enum ePriority{
		CANCEL_TASK,
		MENU,
	};
	enum eValue{
		PHASE,
		VICTORY,
		SEARCH,
		SYSTEM,
		SAVE,
		EXIT,
	};
	enum eYes{
		YES,
		NO,
	};
	
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	// メニュー用
	void eventCircle(int nState,Task::CTaskContext* pContext);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);
	// フェーズ用
	void eventPhaseOK(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);
	void eventPhaseCancel(Task::CTaskContext* pContext);

	// 操作
	void actionCall(int nState,Task::CTaskContext* pContext);

private:
	int nCall_;
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end
