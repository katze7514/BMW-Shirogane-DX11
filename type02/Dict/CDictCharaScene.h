/*
	katze 08/08/18
	キャラ辞典シーン
*/
#pragma once

#include "../Scene/CScene.h"
#include "CDictCharaContext.h"

namespace BMW{
namespace Dict{

class CDictCharaScene : public Scene::CScene<CDictCharaContext>
{
public:
	enum eState{
		FADE,
		FADE_END,
		NORMAL,
		DEMO,
		DEMO_JUMP,
		END,
		INIT_READY,
		INIT,
		INIT_END,
	};
	enum ePriority{
		CTRL,
	};
	enum eScene{
		SELECT,
		VIEW,
#ifdef BMW_DEBUG
		FACE,
		SYMBOL,
#endif
	};

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントリスナ
	void eventFade(Task::CTaskContext*);

private:
	void setFadeOut();

	// キャラ辞典xml読み込み
	void setDictChara();

	// ロードタイマー
	CTimer loadTimer_;
};

} // namespace Dict end
} // namespace BMW end
