/**
	katze 08/08/19
	キャラ辞典シーンファクトリ
*/
#pragma once

namespace BMW{
namespace Dict{

class CDictCharaFactory : public Task::ITaskListFactory
{
public:
	smart_ptr<Task::ITaskList> createTaskList(int nID);
};

} // namesapce Dict end
} // namespace BMW end