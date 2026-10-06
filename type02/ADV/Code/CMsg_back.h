/*
	katze 07/02/17
	メッセージ変更
*/
#pragma once

#include "../CADVMsgLog.h"

namespace BMW{
namespace ADV{
class CADVContext;

namespace API{

class CMsg_back : public Task::ITaskList
{/**
	バックログ
 */
public:
	enum eState
	{
		WAIT,
		END,
	};
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	CADVContext* p;
	list<CADVMsgLog>::iterator it_;

	// 現在の状況を保存するためのもの
	COLORREF	rgb_;			// 現在の文字色
	int			nMsgState_[2];	// 現在のメッセージ状態。有効(1)/無効(0)/非表示(-1)

	// 現在のイテレータにあわせてボード変更
	void changeMsg();
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end