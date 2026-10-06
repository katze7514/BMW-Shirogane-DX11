/*
	katze 05/05/15
	update 06/03/23
	デモ背景の基底
*/
#pragma once

namespace BMW{
namespace Demo{

class IDemoBack : public Task::ITaskBase
{/**
	デモ背景の基底

	ようは、後景と前景を扱うための制御クラス
 */
public:
	// デストラクタ
	virtual ~IDemoBack(){}

	// タスク
	virtual void TaskBack(Task::CTaskContext*)=0;
	virtual void TaskForward(Task::CTaskContext*){}
};

} // namespace Demo end
} // namespace BMW end