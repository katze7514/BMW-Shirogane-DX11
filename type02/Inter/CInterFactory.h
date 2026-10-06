/*
	katze 05/06/16
	インターミッションサブシーン
	ファクトリ
*/
#pragma once

namespace BMW{
namespace Inter{

class CInterFactory : public Task::ITaskListFactory
{/**
	インターミッションサブシーン
	ファクトリ
 */
public:
	smart_ptr<Task::ITaskList> createTaskList(int nID);
};

} // namespace Inter end
} // namespace BMW end