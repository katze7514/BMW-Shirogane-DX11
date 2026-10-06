/*
	katze 05/07/24
	update 06/02/24
	YES/NOダイアログ
*/
#pragma once

namespace BMW{
namespace Unit{

class CYesNoUnit : public Task::CTaskBase
{/**
	YES/NOダイアログ
 */
public:
	typedef delegate<void,Task::CTaskContext*> CancelEvent;
	enum eState{
		NORMAL,
		INTRO,
		END,
	};
	enum eAsk{
		SAVE,
		SALLY,
		PHASE,
		CONTINUE,
		SALLY2,
	};
	// デストラクタ
	virtual ~CYesNoUnit();
	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnInit(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);

	// 設定・取得
	bool IsValid(){ return pPanel_->IsValid(); }
	void valid(bool bV){ pPanel_->valid(bV); }
	bool IsVisible(){ return pPanel_->IsVisible(); }
	void visible(bool bV){ pPanel_->visible(bV); }

	// 設定
	void setAsk(int nAsk);
	void setIntro(int nAsk,Task::CTaskContext* pContext,int nNum=0);
	void setButtonHandler(const GUI::CButton::ButtonEvent& fun, int nYes, int nNo);
	void setCancelHandler(const CancelEvent& fun){ fun_=fun; }

	// イベントハンドラ
	void eventCursol(){}

protected:
	GUI::CPanel*		pPanel_;
	CInteriorCounter	c_;
	CancelEvent			fun_;
};

} // namespace Unit end
} // namespace BMW end