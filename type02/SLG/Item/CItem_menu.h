/*
	katze 05/07/09
	update 06/02/23
	アイテムメニュ
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{
namespace SLG{
namespace Item{

class CItem_menu : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
{/**
	精神メニュ
 */
public:
	enum eState{
		NORMAL,
		CANCEL,
		OK,
	};
	enum ePriority{
		CANCEL_T,
		MENU,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionMenu(int nState, Task::CTaskContext* pContext);

	// イベントハンドラ
	void eventCircle(int nState,Task::CTaskContext* pContext);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);
};

} // namespace Item end
} // namespace SLG end
} // namespace BMW end