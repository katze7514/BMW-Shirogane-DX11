/*
	katze 05/04/25
	genereted by code_gen.rb
	wait
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_wait : public Rule::IRuleTask
{/**
	wait

	waitするフレーム数はStateで代用
 */
public:
	// コンストラクタ
	CCode_wait():nFrame_(1){}
	// タスク
	void OnAction(Task::CTaskContext*);

protected:
	int nFrame_;
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
