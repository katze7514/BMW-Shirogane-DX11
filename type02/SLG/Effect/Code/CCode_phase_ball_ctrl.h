/*
	katze 06/05/24
	フェーズボールの出し入れ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_phase_ball_ctrl : public BMW::Rule::IRuleTask
{/*
	シンボル投入

	出し入れフラグ
	
	が積まれている
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namepsace SLG end
} // namepsace BMW end