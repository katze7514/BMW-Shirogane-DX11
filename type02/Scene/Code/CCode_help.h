/*
	katze 07/03/18
	ヘルプウインドウ
*/
#pragma once

namespace BMW{
namespace Scene{
namespace Code{

class CCode_help : public Rule::IRuleTask
{/**
	ヘルプウインドウ

	スタックに
		ヘルプID
	を積んでおく
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Scene end
} // namespace BMW end