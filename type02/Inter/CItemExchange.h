/*
	katze 06/03/02
	アイテム交換
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CItemExchange : public ITrainBase
{/**
	アイテム交換
 */
public:
	enum eState{
		NORMAL,
		END,
	};
	// タスク
	void Task(Task::CTaskContext* pContext);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionOK(Task::CTaskContext*);
	void actionStatus(Task::CTaskContext*);
	void actionBack(Task::CTaskContext*);

private:
	int anCharaID_[9];
	// インターフェイス
	// GUI::CPanel* pPanel_;
	// キャラ一覧
	GUI::CPanel* pView_;
	// ステータス
	GUI::CPanel* pNowStatus_;
	GUI::CPanel* pUpStatus_;
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end