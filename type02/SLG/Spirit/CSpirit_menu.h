/*
	katze 05/07/01
	update 06/02/12
	精神メニュ
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{

namespace GUI{
class CGage;
} // namespace GUI end

namespace SLG{
namespace Spirit{

class CSpirit_menu : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
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
		SP,
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

private:
	GUI::CPanel* pSp_;
	GUI::CGage*	pSpGage_;
};

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end