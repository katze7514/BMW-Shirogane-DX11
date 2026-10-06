/*
	katze 05/03/07
	トップレベルシーンのファクトリ
*/
#pragma once

namespace BMW{
namespace Scene{

class CSceneFactory : public Task::ITaskListFactory
{/**
	トップレベルシーンのファクトリ
 */
public:
	// デストラクタ
	virtual ~CSceneFactory(){}

	smart_ptr<Task::ITaskList> createTaskList(int nID);
};


} // namespace Scene end
} // namespace BMW end