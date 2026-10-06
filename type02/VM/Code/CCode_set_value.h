/*
	katze 05/04/25
	genereted by code_gen.rb
	set_value
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_set_value : public Rule::IRuleTask
{/**
	set_value
 */
public:
	// コンストラクタ
	CCode_set_value(int nFlag){ setState(nFlag); }
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
