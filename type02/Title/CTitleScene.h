/*
	katze 05/05/19
	update 06/02/02
	タイトルシーン
*/
#pragma once

#include "../Scene/CScene.h"
#include "../Scene/Event/IListenerCircleMenu.h"

namespace BMW{
namespace Title{

class CTitleScene : public Scene::CScene<Task::CTaskContext>
{/**
	タイトルシーン
 */
public:
	enum eState{
		NORMAL,
		FADE,
		NEW,
		LOAD,
		CONTINUE,
		EXIT,
		CHARA,
		SOUND,
	#ifdef BMW_DEBUG
		DEMO,
	#endif
	};
	enum ePriority{
		INTERFACE,
	};
	enum eMenu{
		NEW_M,
		LOAD_M,
		CONTINUE_M,
		EXIT_M,
		CHARA_M,
		SOUND_M,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);
	void eventFader(Task::CTaskContext*);

#ifdef BMW_DEBUG
	void setDemo(Task::CTaskContext*);
#endif

private:
	int nNext_;
	bool bCont_;
	// 全体パネル
	GUI::CPanel* pPanel_;

	void setContinue(GUI::CButton* pButton, Task::CTaskContext*);
};

} // namespace Title end
} // namespace BMW end