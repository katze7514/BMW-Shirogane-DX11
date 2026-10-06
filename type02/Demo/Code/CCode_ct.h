/*
	katze 06/05/21
	クリティカル表示コード
*/
#pragma once

#include "../CDemoContext.h"
#include "../GUI/CDemoDamage.h"

namespace BMW{
namespace Demo{
namespace Code{

class CCode_ct : public Rule::IRuleTask
{/**
	クリティカル表示コード
 */
public:
	void OnAction(Task::CTaskContext* pContext)
	{
		// ctを呼び出すだけ
		static_cast<CDemoContext*>(pContext)->getDamage()->ct();
	}
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end