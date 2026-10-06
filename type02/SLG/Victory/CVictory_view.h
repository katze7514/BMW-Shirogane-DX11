/*
	katze 05/05/21
	勝利条件表示
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Victory{

class CVictory_view : public BMW::Rule::CRuleList
{/**
	勝利条件表示

	但し、これはスケルトン
	実際には現在の勝利条件IDに合わせて、表示を変更する
 */
public:
	enum eState{
		NORMAL,
		CLICK,
		TUTORIAL,
	};
	enum ePriority{
		OK,
		CANCEL,
		BACK,
	};
	// デストラクタ
	virtual ~CVictory_view(){}
	// タスク
	virtual void OnReset(Task::CTaskContext*);
	virtual void OnInit(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);

private:
	GUI::CPanelCtrl* pVictory_;
	GUI::CPanelCtrl* pLose_;
	GUI::CPanelCtrl* pExpert_;
};

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end