/*
	katze 05/04/30
	説得キャラ設定
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Sally{

class CSally_set_pers : public BMW::Rule::IRuleTask
{/**
	説得キャラ設定

	設定先キャラID(SLG ID)は、TargetCharaに設定しておく
	スタックに、
	  設定キャラID
	を積んでおく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end