/*
	katze 05/04/25
	genereted by code_gen.rb
	jump
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_jump : public Rule::IRuleTask
{/**
	jump

	飛び先は、Stateで代用
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
