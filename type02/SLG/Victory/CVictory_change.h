/*
	katze 05/07/19
	勝利条件判定についての変更通知
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Victory{

class CVictory_change : public BMW::Rule::CRuleList
{/**
	勝利条件判定についての変更通知

	熟練度取ったとか、
	勝利条件変わったとか、そういうこと
 */
public:
	enum eState{
		//CLICK,
		NORMAL,
		END,
	};
	enum ePriority{
		//OK,
		BACK,
	};
	enum eChange{
		VICTORY,
		LOSE,
		EXPERT,
		EXPERT_GET,
	};
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

private:
	GUI::CPanel*		pPanel_;
	GUI::CPanelCtrl*	pText_;

	// 変更タイプ
	int nType_;

	void updatePanel(bool bVic[]);
};

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end