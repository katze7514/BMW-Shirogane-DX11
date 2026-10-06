/*
	katze 05/07/09
	補給適用
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Refill{

class CRefill_apply : public Task::ITaskList
{/**
	補給適用
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Refill end
} // namespace SLG end
} // namespace BMW end