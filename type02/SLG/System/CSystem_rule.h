/*
	katze 05/07/06
	update 06/02/22
	システム
*/
#pragma once

namespace BMW{
namespace SLG{
namespace System{

class CSystem_rule : public BMW::Rule::CRuleList
{/**
	システム
 */
public:
	enum eState{
		NORMAL,
		CANCEL,
	};
	enum ePriority{
		CANCEL_T,
		PANEL,
	};
	enum eOnOff{
		GRID_ON,
		GRID_OFF,
		FRAME_ON,
		FRAME_OFF,
		DEMO_ON,
		DEMO_OFF,
		HELP_ON,
		HELP_OFF,
	};
	enum eUpDown{
		BGM_0,
		BGM_1,
		BGM_2,
		BGM_3,
		BGM_4,
		BGM_5,
		BGM_6,
		BGM_7,
		BGM_8,
		BGM_9,
		BGM_10,
		SE_0,
		SE_1,
		SE_2,
		SE_3,
		SE_4,
		SE_5,
		SE_6,
		SE_7,
		SE_8,
		SE_9,
		SE_10,
	};
	// コンストラクタ
	CSystem_rule():pCurBgm_(NULL),pCurSe_(NULL){}
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionOK(Task::CTaskContext*);

	// イベントハンドラ
	void eventOnOff(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventUpDown(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventOK(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	// グリッド
	GUI::CPanel* pGrid_;
	// フレーム
	GUI::CPanel* pFrame_;
	// デモOFF
	GUI::CPanel* pDemo_;
	// ヘルプ
	GUI::CPanel* pHelp_;
	// バー
	int nBgm_,nSe_; // 段階
	GUI::CPanel*	pBgm_;
	GUI::CButton*	pCurBgm_;
	GUI::CPanel*	pSe_;
	GUI::CButton*	pCurSe_;

	// ボリュームボタンいじり
	void setBgmVolume();
	void setSeVolume();
};

} // namespace System end
} // namespace SLG end
} // namespace BMW end