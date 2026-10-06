/*
	katze 06/07/23
	フレームスキップフラグ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_skip : public BMW::Rule::IRuleTask
{/**
	フレームスキップフラグ

	フラグ
	
	が積まれてる
 */
public:
	enum eState
	{
		RESET=-1,
		FALSE_SKIP,
		TRUE_SKIP,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end
