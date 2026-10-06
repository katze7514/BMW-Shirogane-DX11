/*
	katze 05/07/20
	pop
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_pop : public Rule::IRuleTask
{/**
	pop
 */
public:
	// É^ÉXÉN
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
