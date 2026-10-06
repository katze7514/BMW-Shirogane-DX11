/*
	katze 05/05/20
	SEの再生終了をまつ
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Sound{
namespace Code{

class CCode_se_wait : public Task::ITaskList
{/**
	SEの再生終了をまつ
 */
public:
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	int nID_;
	bool bInput_;
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
