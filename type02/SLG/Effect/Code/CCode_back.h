/*
	katze 06/06/25
	マップ背景の入れ替え
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_back : public BMW::Rule::IRuleTask
{/**
	マップ背景の入れ替え

	ロードするシンボルID
	
	が積まれてる
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end
