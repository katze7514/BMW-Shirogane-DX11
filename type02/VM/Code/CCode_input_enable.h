/*
	katze 05/04/25
	genereted by code_gen.rb
	input_enable
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_input_enable : public Rule::IRuleTask
{/**
	input_enable

	enableフラグはstateで代用。1なら有効、0なら無効
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
