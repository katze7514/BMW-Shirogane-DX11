/*
	katze 05/05/19
	メッセージ変更
*/
#pragma once

namespace BMW{
namespace ADV{
namespace API{

class CMsg_change : public Task::ITaskList
{/**
	メッセージ変更

	これを呼び出す前に

	サイド
	キャラID
	顔ID
	文字列プールID
	マスクフラグ

	の順にスタックに積んでおく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end