/*
	katze 05/07/20
	タスクリストの親のStateを変更する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_parent_state : public BMW::Rule::IRuleTask
{/**
	タスクリストの親のStateを変更する
 */
public:
	// コンストラクタ
	CCode_parent_state(int nState=0){ setState(nState); }
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end