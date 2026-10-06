#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CMenu_select_player.h"
#include "CMenu_select_Cancel.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_select_Cancel::OnAction(Task::CTaskContext* pContext)
{
	if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
	{// キャンセルされた
		if(pContext->getValue(Flag::TARGET_MAP)>=0)
		{// 確かに押されてる
			// じゃ、ステータスね
			getParent()->setState(CMenu_select_player::STATUS);
		}
	}
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end