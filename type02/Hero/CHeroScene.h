/*
	katze 05/06/28
	update 06/03/02
	主人公選択
*/
#pragma once

#include "../Scene/CScene.h"

namespace BMW{
namespace Hero{

class CHeroScene : public Scene::CScene<>
{/**
	主人公選択
 */
public:
	enum eState{
		NORMAL,
		INTRO,
		END,
		GAME,
		CHANGE_OUT,
		CHANGE_IN,
	};
	enum eButton{
		TAKUMI,
		HARUNA,
		OK,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionSave(Task::CTaskContext*);

private:

	// インターフェイス
	GUI::CPanel*		pPanel_;
	GUI::CButton*		pTakumi_;
	GUI::CButton*		pHaruna_;
	GUI::CPanelCtrl*	pStand_;
	GUI::CPanelCtrl*	pProfile_;

	Movie::CMotion		motion_[2];
	string				sID_;
};

} // namespace Hero end
} // namespace BMW end