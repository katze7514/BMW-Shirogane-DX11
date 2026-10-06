/*
	katze 05/04/25
	genereted by code_gen.rb
	get_value
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_get_value : public Rule::IRuleTask
{/**
	get_value

	呼び出すデータIDはStateで代用
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
