/*
	katze 06/07/10
	エンディングシーン
*/
#pragma once

#include "../Scene/CScene.h"

namespace BMW{
namespace Unit{
class CYesNoUnit;
} // namespace Unit end

namespace ED{

class CEdScene : public Scene::CScene<>
{/**
	エンディングシーン
 */
public:
	enum ePriority{
		BACK,
		MOVIE,
		YESNO,
	};
	enum eState{
		FADE,
		PLAY,
		PLAY_WAIT,
		WAIT,
		DIALOG,
		DIALOG_WAIT,
		DATA,
		DATA_WAIT,
		END,
		END_WAIT,
	};
	enum eYesNo{
		YES,NO
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントリスナ
	void eventFade(Task::CTaskContext*);
	void eventYesNo(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);
	void eventCancel(Task::CTaskContext*);

private:
	Movie::CMovieClip* pED_;
	Unit::CYesNoUnit* pYesNo_;

	int nReturn_;
	int nFrame_;
};

} // namespace ED end
} // namespace BMW end