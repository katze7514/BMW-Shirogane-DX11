/*
	katze 05/03/24
	SLGシーンクラス
*/
#pragma once

#include "../Scene/CScene.h"
#include "Context/CSLGContext.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{

class CSLGScene : public Scene::CScene<CSLGContext>
{/**
	SLGシーンクラス
 */
public:
	enum eState
	{
		NORMAL,
		FADE,
		DEMO,
		DEMO_FADE,
		DEMO_END,
		END,
		FADE_END,
		FADE_C,
	};
	enum ePriority
	{// タスクプライオリティ
		MAP,
		EVENT,
		VM,
		TASK,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// コンテニューセーブ
	void continueSave();
	
	// イベントハンドラ
	void eventFade(Task::CTaskContext*);

	// キャラ追加
	//void addChara(int nID);
	Task::ITaskBase* addChara2(int nID);

private:
	// 初期化の切り分け
	void newInit(Task::CTaskContext*);
	void continueInit(Task::CTaskContext*);
	// マップへのキャラの追加
	void addCharaList(Map::CMap* pMap, list<int>& List);
	// フェードイベントハンドラ
	Scene::CFoward::FaderEvent fun;
	// キーボード直触るため
	Input::CTaskInput* pInput_;

#ifdef BMW_DEBUG
	void slgDebugCmd();

#ifdef STAGE_CREATE
	int DEBUG_ADD_SLG_ID_START;
	static int DEBUG_ADD_SLG_ID_START_CONST;
#endif

#endif
};

} // namespace SLG end
} // namespace Task end