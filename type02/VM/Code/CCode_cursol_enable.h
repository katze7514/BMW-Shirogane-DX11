/*
	katze 05/04/25
	genereted by code_gen.rb
	cursol_enable
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_cursol_enable : public Rule::IRuleTask
{/**
	cursol_enable

	設定する値をVisibleにいれておく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
