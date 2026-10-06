/*
	katze 05/05/01
	キャラデータを削除する
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Sally{

class CSally_del_chara : public Task::ITaskList
{/**
	キャラデータを削除する
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end