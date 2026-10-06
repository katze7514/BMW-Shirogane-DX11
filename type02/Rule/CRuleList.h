/*
	katze 05/03/28
	Œã”»’èŒ^TaskList
*/
#pragma once

#include "../Task/CTaskList.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace Rule{

class CRuleList : public Task::CTaskList
{/**
	e‚ªq‚ÌŒã‚Éˆ—‚ğs‚¤TaskList
 */
public:
	virtual ~CRuleList(){}
	virtual void Task(Task::CTaskContext*);
};

} // namespace Rule end
} // namespace BMW end