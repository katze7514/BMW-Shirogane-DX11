/*
	katze 05/06/29
	MSGボードの状態を設定する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Event{

class CEvent_Msg_state : public BMW::Rule::IRuleTask
{/**
	MSGボードの状態を設定する

	つっても、VISIBLEだけだけど
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end