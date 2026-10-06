/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	get_target
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Code{

class CCode_get_target : public BMW::Rule::IRuleTask
{/**
	get_target

	マップかキャラかは、Stateで代用
 */
public:
	enum eState{
		CHARA,
		MAP,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
