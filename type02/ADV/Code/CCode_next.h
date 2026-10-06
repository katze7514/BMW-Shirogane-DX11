/*
	katze 05/07/18
	次のシナリオを設定する
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CCode_next : public BMW::Rule::IRuleTask
{/**
 	次のシナリオを設定する
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespsace ADV end
} // namespace BMW end