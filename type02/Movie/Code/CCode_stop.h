/*
	katze 05/05/25
	フレームをストップする
*/
#pragma once

namespace BMW{
namespace Movie{
namespace Code{

class CCode_stop : public BMW::Rule::IRuleTask
{/**
	こいつを保持してるキーフレームをstopする
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};


} // namespace Code end
} // namepsace Movie end
} // nameosace BMW end