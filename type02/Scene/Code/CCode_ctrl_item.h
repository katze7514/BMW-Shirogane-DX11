/*
	katze 05/06/29
	アイテム操作
*/
#pragma once

namespace BMW{
namespace Scene{
namespace Code{

class CCode_ctrl_item : public Rule::IRuleTask
{/**
	アイテム操作

	スタックに
		増減数
		アイテムID
	と積んでおく
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Scene end
} // namespace BMW end