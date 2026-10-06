/*
	katze 05/07/01
	精神対象選択
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Spirit{

class CSpirit_select : public Task::CTaskList
{/**
	精神対象選択
 */
public:
	enum eState{
		NORMAL,
		OK,
		CANCEL,
		RETURN,
	};
	enum ePriority{
		CANCEL_T,
		OK_T,
	};
	// コンストラクタ
	CSpirit_select():nID_(-1),nSide_(-1){}
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionReturn(Task::CTaskContext*);

	// 範囲設定
	void rangeFriend(int nID,CSLGContext*);
	void rangeEnemy(int nID,CSLGContext*);
	void rangeSet(int nID,list<int>& List,CSLGContext*);

private:
	int nID_;
	int nSide_;
	CSLGContext* p;

	// ステータス
	void changeStatus(CSLGContext*);
	void actionInValid(CSLGContext* p);
};

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end