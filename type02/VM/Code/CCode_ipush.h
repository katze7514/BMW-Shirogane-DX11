/*
	katze 05/04/25
	genereted by code_gen.rb
	ipush
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_ipush : public Rule::IRuleTask
{/**
	ipush

	pushする値は、Stateで代用
 */
public:
	// コンストラクタ
	CCode_ipush(int n=0){ setState(n); }
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
