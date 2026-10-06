/*
	katze 06/05/23
	シンボル削除
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_del_symbol : public BMW::Rule::IRuleTask
{/**
	シンボル削除

	No
	
	が積まれてる
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end
