/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	menu_select
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{

namespace Effect{
class CEffectMovieClip;
} // namespace Effecnt end

namespace Menu{
class CGameOverUnit;
class CMenu_select : public Task::ITaskList
{/**
	menu_select

	メニューセレクタ
 */
public:
	enum eState{
		NORMAL,			// 通常動作
		PHASE_INIT,
		PHASE,			// フェーズ切り替え
		END_EFFECT,		// 終了時エフェクト
		END_DIALOG,
		END_CONTINUE,	// コンテニュー終了
		END,			// SLG終了
		TITLE,			// タイトルに戻る
	};
	// デストラクタ
	~CMenu_select();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アクション
	void changePhase(Task::CTaskContext* pContext);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

private:
	Task::CTaskCtrl<Effect::CEffectMovieClip>* pEndCtrl_;

	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);

	CGameOverUnit* pUnit_;
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end
