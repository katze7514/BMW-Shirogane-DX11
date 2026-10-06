/*
	katze 07/02/17
	メッセージ変更
*/
#pragma once

#include "CSLGMsgLog.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Event{

class CEvent_Msg_back : public Task::ITaskList
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
	CSLGContext* p;
	list<CSLGMsgLog>::iterator it_;

	// 現在の状況を保存するためのもの
	COLORREF	rgb_;			// 現在の文字色

	// 現在のイテレータにあわせてボード変更
	void changeMsg();
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end