/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	move_view
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Move{

class CMove_view : public BMW::Rule::IRuleTask
{/**
	move_view

	on/offは、visibleで代用
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
