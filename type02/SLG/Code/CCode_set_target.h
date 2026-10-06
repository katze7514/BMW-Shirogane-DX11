/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	set_target
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Code{

class CCode_set_target : public BMW::Rule::IRuleTask
{/**
	set_target

	マップかキャラかはstateで代用
	設定する値は、スタックに積んでおく
 */
public:
	enum eState{
		CHARA,
		MAP
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
