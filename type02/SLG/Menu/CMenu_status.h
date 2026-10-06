/*
	katze 05/05/02
	update 06/02/23
	ステータスメニュー
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Menu{

class CMenu_status : public BMW::Rule::CRuleList
{/**
	ステータスメニュー

	状況に合わせて、簡易ステータスや地形情報表示を
	切り分ける
 */
public:
	enum eState{
		NORMAL,
		OK,
		CANCEL,
	};
	enum ePriority{
		OK_T,
		CANCEL_T,
		STATUS,
		UNKNOWN,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アップデート
	void updateLand(CSLGContext*);
	void updateChara(CSLGContext*);

private:
	GUI::CPanel* pStatus_;
	GUI::CPanel* pChara_;
	GUI::CPanel* pUnKnown_;
	GUI::CPanel* pJotai_;
	GUI::CPanel* pEngo_;
	GUI::CPanel* pSpirit_;
	GUI::CPanel* pLand_;
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end