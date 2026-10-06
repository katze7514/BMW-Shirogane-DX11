/*
	katze 05/04/25
	genereted by code_gen.rb
	if_seq
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_if_seq : public Rule::IRuleTask
{/**
	if_seq

	飛び先のPcはStateで代用
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
