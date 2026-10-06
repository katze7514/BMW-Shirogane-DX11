#include "stdafx.h"

#include "IDemoBackLine.h"
#include "CDemoBack.h"

namespace BMW{
namespace Demo{

CDemoBack::~CDemoBack()
{
	for_each(vecBack_.begin(),vecBack_.end(),DeleteObj());
	vecBack_.clear();

	for_each(vecForward_.begin(),vecForward_.end(),DeleteObj());
	vecForward_.clear();
}

void CDemoBack::Task(Task::CTaskContext* pContext)
{
	TaskBack(pContext);
	TaskForward(pContext);
}

void CDemoBack::TaskBack(Task::CTaskContext* pContext)
{
	for(int i=0; i<(int)vecBack_.size(); ++i)
		vecBack_[i]->Task(pContext);
}

void CDemoBack::TaskForward(Task::CTaskContext* pContext)
{
	for(int i=0; i<(int)vecForward_.size(); ++i)
		vecForward_[i]->Task(pContext);
}

} // namespace Demo end
} // namespace BMW end