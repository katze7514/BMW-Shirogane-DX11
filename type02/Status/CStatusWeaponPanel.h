/*
	katze 06/02/22
	武器ステータスパネル
*/
#pragma once

namespace BMW{
namespace Status{

class CStatusWeaponPanel : public Task::CTaskBase
{/**
	武器ステータスパネル

	中身の武器データは、こいつの外で設定する。
	こいつはあくまで、武器ステータスの動作を定義しているだけ
 */
public:
	// デストラクタ
	virtual ~CStatusWeaponPanel(){}

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	
	// 設定
	bool IsValid()const{ return pPanel_->IsValid(); }
	void valid(bool bV){ pPanel_->valid(bV); }
	bool IsVisible()const{ return pPanel_->IsVisible(); }
	void visible(bool bV){ pPanel_->visible(bV); }

	// イベントハンドラ
	void eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

	// パネルゲット
	GUI::CPanel*		getPanel(){ return pPanel_; }
	GUI::CPanelCtrl*	getDetail(){ return pDetail_; }

private:
	// WEAPON_PANEL
	GUI::CPanel* pPanel_;
	// DETAIL
	GUI::CPanelCtrl* pDetail_;
};

} // namespace Status end
} // namespace BMW end