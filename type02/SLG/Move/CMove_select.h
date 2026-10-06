/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	move_select
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Move{

class CMove_select : public BMW::Rule::CRuleList
{/**
	move_select
 */
public:
	enum eState{
		NORMAL,
		OK,
		CANCEL,
	};
	enum ePriority{
		OK_T,
		CANCEL_T,
	};

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionEnd(Task::CTaskContext*);
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
