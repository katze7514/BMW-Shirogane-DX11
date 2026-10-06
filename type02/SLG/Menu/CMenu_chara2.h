/*
	katze 06/06/09
	キャラメニューVer.2
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;

namespace Menu{

class CMenu_chara2 : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
{/**
	キャラメニュー
 */
public:
	enum eState{
		NORMAL, // 通常
		CANCEL, // キャンセルが押された
		CALL,
	};
	enum ePrioriy{
		CANCEL_T,	// キャンセル監視
		MENU,
	};
	enum eButton{
		MOVE,	// 移動が選択された
		ATTACK, // 攻撃が選択された
		CURE,	// 治癒が選択された
		REFILL, // 補給が選択された
		SPIRIT,	// 精神が選択された
		ITEM,	// アイテムが選択された
		STATUS,	// 能力が選択された
		LOVE,	// 説得が選択された
		WAIT,	// 待機が選択された
	};
	// コンストラクタ・デストラクタ
	//virtual ~CMenu_chara2();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	void eventCircle(int nState,Task::CTaskContext* pContext);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);

	// 操作
	void actionCall(int nState,Task::CTaskContext* pContext);
	// 説得表示判定
	bool actionPers(CDataCharaSLG* pChara, CSLGContext* p);

private:
	int nCall_;

	// サークルメニュー設定
	void updateMenu(CSLGContext* p);

	// アタックボタンの×
	Task::ITaskBase* pAtkBatsu_;
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end
