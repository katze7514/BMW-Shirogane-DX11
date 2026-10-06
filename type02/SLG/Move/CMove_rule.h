/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	move_rule
*/
#pragma once

#include "../Context/CCharaState.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
class CSLGContext;
namespace Move{

class CMove_rule : public Task::ITaskList
{/**
	move_rule

	Playerの移動ルールを実行する
 */
public:
	enum eState{
		RANGE,
		CURSOL,
		SELECT,
		ROAD,
		EXEC,
		MENU,
		CANCEL,
		END,
	};
	// デストラクタ
	virtual ~CMove_rule(){}
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID,Task::CTaskContext*);

	// 移動データをキャンセルする
	void moveCancel(Task::CTaskContext*);

	// 設定・取得
	const CCharaState&	getPrevState() const { return prevState_; }
	void				setPrevState(const CCharaState& state){ prevState_=state; }	

	// 精神フラグ倒し
	void flagSpirit(Task::CTaskContext*);

protected:
	// 一個前の状態
	CCharaState prevState_;
	int nTargetMap_;	// 移動先Target
	CSLGContext* p;
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
