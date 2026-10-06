/*
	katze 05/05/26
	SEの再生終了待ちをする
*/
#pragma once

namespace BMW{
namespace Movie{
namespace Code{

class CCode_se_wait : public BMW::Rule::IRuleTask
{/**
	SEの再生終了待ちをする

	stateに設定されているSE IDのSEの再生終了待ちをする
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namesapce Code end
} // namespace Movie end
} // namespace BMW end