/*
	katze 05/03/27
	ルールの基底クラス
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace Rule{

class IRuleTask : public Task::ITaskBase
{/**
	ルールの基底クラス
	描画を行わないだけ
 */
public:
	virtual ~IRuleTask(){}
	void Task(Task::CTaskContext* pContext)
	{
		if(pContext->IsAction() && IsValid())
			OnAction(pContext);
	}
};

} // namespace Rule end
} // namespace BMW end