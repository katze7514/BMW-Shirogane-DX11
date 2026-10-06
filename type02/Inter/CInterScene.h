/*
	katze 05/06/15
	インターミッションシーン
*/
#pragma once

#include "../Scene/CScene.h"
#include "CInterContext.h"

namespace BMW{
namespace Inter{

class CInterScene : public BMW::Scene::CScene<CInterContext>
{/**
	インターミッションシーン
 */
public:
	enum eState{
		NORMAL,
		FADE,
		DATA,
		END,
		FADE_END,
	};
	enum ePriority{
		CTRL,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID,Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);

private:
	// 設定
	void setItem();
	void setChara();

	// 変身対応キャラを扱う
	void setMetamor(int& nID, CInterChara* pChara);
};

} // namespace Inter end
} // namespace BMW end