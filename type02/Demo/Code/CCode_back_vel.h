/*
	katze 05/05/22
	背景速度を変更するコード
*/
#pragma once

namespace BMW{
namespace Demo{
namespace Code{

class CCode_back_vel : public BMW::Rule::IRuleTask
{/**
	背景速度を変更するコード

	変更速度はStateで代用
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end