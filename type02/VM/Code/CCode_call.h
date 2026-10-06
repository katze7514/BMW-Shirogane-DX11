/*
	katze 05/04/25
	genereted by code_gen.rb
	call
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_call : public Rule::IRuleTask
{/**
	call

	// 呼び出す関数IDはStateで代用
 */
public:
	// コンストラクタ
	CCode_call(int n=0){ setState(n); }
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
