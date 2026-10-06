/*
	katze 05/05/23
	メッセージ変更
*/
#pragma once

#include "../../ADV/AdvCmd.h"

namespace BMW{
namespace Demo{
namespace Code{

class CCode_msg_demo : public BMW::Rule::IRuleTask
{/**
	メッセージ変更
 */
public:
	// アクション
	void	OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end