/*
	katze 05/03/07
	TaskListファクトリの基底クラス
*/
#pragma once

namespace BMW{
namespace Task{

class ITaskList;
class ITaskListFactory
{/**
	ファクトリーインターフェイス
 */
public:
	// デストラクタ
	virtual ~ITaskListFactory(){}

	// 指定されたIDのITaskListを生成して返す
	virtual smart_ptr<ITaskList> createTaskList(int nID)=0;
};

} // namespace Task end
} // namespace BMW end