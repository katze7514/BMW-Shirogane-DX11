/*
	katze 05/04/25
	genereted by code_gen.rb
	se
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Sound{
namespace Code{

class CCode_se : public Rule::IRuleTask
{/**
	se

	SE再生指定
	stateで操作を指定する
	操作対象のSE IDをスタックトップに積んでおく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Sounds end
} // namespace BMW end
