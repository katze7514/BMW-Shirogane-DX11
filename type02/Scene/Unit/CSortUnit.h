/*
	katze 06/02/25
	ソートユニット
*/
#pragma once

#include "../Event/IListenerCircleMenu.h"

namespace BMW{
namespace Unit{

class CSortUnit : public Task::CTaskBase, public Event::IListenerCircleMenu
{/**
	ソートユニット
 */
public:
	typedef delegate<void,int,int,Task::CTaskContext*> SortEvent;
	enum eState{
		NORMAL,
		CANCEL,
		WAIT,	// 動作ウェイト
	};
	enum eButton{
		ID,
		LV,
		HP,
		EN,
		SP,
		KI,
		NEXT,
	};
	enum eOrder{
		UP,
		DOWN,
	};
	enum eSort{
		KEY,
		ORDER,
	};
	enum eEvent{
		START=-2,
		END,
	};
	// デストラクタ
	virtual ~CSortUnit();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	// 表示が与えれた状況になる
	void actionSort();
	void actionKey(Task::CTaskContext* pContext);

	// 設定・取得
	int	 getKey()const{ return nKey_; }
	void setKey(int nKey){ nKey_=nKey; }
	int	 getOrder()const{ return nOrder_; }
	void setOrder(int nOrder){ nOrder_=nOrder; }
	void setEventHandler(const SortEvent& fun){ fun_=fun; }

	// イベントハンドラ
	void eventCircle(int nState, Task::CTaskContext* pContext);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventSort(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);


private:
	// 現在の状況
	int nKey_;
	int nOrder_;

	// 変更が合った場合に呼ぶイベントハンドラ
	SortEvent fun_;

	Rule::CRuleCancel* pCancel_;
	// インターフェイス
	GUI::CPanel* pPanel_;
	// 中央のボタン
	GUI::CPanelCtrl* pKeyButton_;
	// 表示テキスト
	GUI::CText* pKeyDisp_;
	// KEY選択のサークルメニュー
	// GUI::CCircleMenu* pMenu_;
	// 矢印
	GUI::CButton* pArrow_;
	// 矢印の動き
	Movie::CMovieClip* pOrderMovie_[2];
	GUI::CPanelCtrl* pOrderDisp_;
};

} // namepsace Unit end
} // namespace BMW end