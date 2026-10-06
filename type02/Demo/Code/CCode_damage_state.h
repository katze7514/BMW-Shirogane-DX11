/*
	katze 05/05/24
	ダメージ
*/
#pragma once

namespace BMW{
namespace Demo{
namespace Code{

class CCode_damage_state : public BMW::Rule::IRuleTask
{/**
	ダメージ
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end