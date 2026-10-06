/*
	katze 06/05/23
	シンボルロード
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_load_symbol : public BMW::Rule::IRuleTask
{/**
	シンボルロード

	No
	ロードするシンボルID
	対象effectDB
	
	が積まれてる
 */
public:
	enum eEffect{
		NORMAL,
		COMMON,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end
