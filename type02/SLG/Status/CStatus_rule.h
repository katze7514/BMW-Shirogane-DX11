/*
	katze 05/05/31
	update 06/02/22
	ステータスルール
*/
#pragma once

namespace BMW{

namespace Status{
class CStatusWeaponPanel;
} // namespace Status end

namespace SLG{
namespace Status{

class CStatus_rule : public BMW::Rule::CRuleList
{/**
	ステータスルール
 */
public:
	enum eState{
		INTRO,
		NORMAL,
		END_C,
		END,
	};
	enum ePriority{
		CANCEL_T,
		PANEL,
	};
	enum eButton{
		BASE,
		WEAPON,
	};
	// デストラクタ
	~CStatus_rule();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	//void eventFader(Task::CTaskContext*);
	void eventMenu(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventCursol();

private:
	GUI::CPanel*						pPanel_;
	GUI::CPanelCtrl*					pStatus_;
	GUI::CPanel*						pWeaponPanel_;
	// ↓とりあえず2つ用意しておく
	BMW::Status::CStatusWeaponPanel*	pWeapon_[2];
	int									nWeapon_; // 現在、表示されてる武器パネル

	void setStatusWeapon(const CDataCharaSLG& chara, CSLGContext& p);
};

} // nmaespace Status end
} // namespace SLG end
} // namespace BMW end