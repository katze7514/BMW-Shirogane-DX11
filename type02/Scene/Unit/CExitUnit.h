/*
	katze 06/02/26
	Exitユニット
*/
#pragma once

namespace BMW{
namespace Unit{

class CExitUnit : public Task::CTaskBase
{/**
	Exitユニット
 */
public:
	typedef delegate<void,int,Task::CTaskContext*> ExitEvent;
	enum eState{
		NORMAL,
		INTRO,
	};
	enum eValue{
		TITLE,
		END,
		CANCEL
	};
	// デストラクタ
	virtual ~CExitUnit();
	// タスク
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	
	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);
	void eventCursol();

	// 設定・取得
	void setEventHandler(const ExitEvent& fun){ fun_=fun; }

private:
	GUI::CPanel*		pPanel_;
	CInteriorCounter	c_;
	ExitEvent			fun_;
	CFastPlane*			plane_;
};

} // namespace Unit end
} // namespace BMW end