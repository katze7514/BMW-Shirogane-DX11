/*
	katze 05/04/26
	メニューセレクト for Player 時のCancel監視タスク
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Menu{

class CMenu_select_Cancel : public BMW::Rule::IRuleTask
{/*
	メニューセレクト for Player 時のCancel監視タスク
*/
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end