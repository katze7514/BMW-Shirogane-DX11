#include "stdafx.h"

#include "../../Scene/GUI/CCircleMenu.h"

#include "CAttack_action.h"
#include "CAttack_weapon_support.h"

namespace BMW{
namespace SLG{
namespace Attack{

////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////
void CAttack_weapon_support::eventCircle(int nState, Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::EXIT)
	{// 動作終了したら、親に通知
		fun_(pContext->top()>=0 ? WEAPON_EVENT : CANCEL_EVENT, pContext);
	}
	else
	{// 登場動作終了
		pContext->getInput()->guard(false);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end