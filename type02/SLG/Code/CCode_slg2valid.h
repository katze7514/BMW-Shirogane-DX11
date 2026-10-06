/*
	katze 07/01/20
	現在のSLGキャラマップにいる味方キャラを
	validリストに設定する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_slg2valid : public BMW::Rule::IRuleTask
{/**
	現在のSLGキャラマップにいる味方キャラを
	validリストに設定する
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
