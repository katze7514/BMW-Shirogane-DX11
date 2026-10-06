/*
	katze 05/04/26
	update 06/02/23
	メニューセレクト for Player 時のNO監視タスク
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Menu{

class CMenu_select_NO : public BMW::Task::ITaskBase
{/*
	メニューセレクト for Player 時のNO監視タスク
*/
public:
	// コンストラクタ
	CMenu_select_NO():nID_(-1),nSide_(-1){}

	// タスク
	void Task(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// ステータス
	void changeStatus(CSLGContext*);
	void actionInValid(CSLGContext* p);

private:
	int nID_;	// 現在セットアップされてるID
	int nSide_; // 現在表示さてるside
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end