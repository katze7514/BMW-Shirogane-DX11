/*
	katze 06/07/18
	ロゴシーン
*/
#pragma once

#include "../Scene/CScene.h"

namespace BMW{
namespace Logo{

class CLogoScene : public Scene::CScene<>
{/**
	ロゴシーン
 */
public:
	enum eState{
		START,
		NORMAL,
		FADE_OUT,
		FADE_IN,
		END,
		INIT_ADV,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);

private:
	int nFrame_;
};

} // namespace Logo end
} // namespace BMW end