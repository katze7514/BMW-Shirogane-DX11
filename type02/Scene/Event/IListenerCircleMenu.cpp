#include "stdafx.h"

#include "../GUI/CCircleMenu.h"

#include "IListenerCircleMenu.h"

namespace BMW{
namespace Event{

////////////////////////////////////////////////
// ƒAƒNƒVƒ‡ƒ“
////////////////////////////////////////////////
void IListenerCircleMenu::actionMenu(int nState, Task::CTaskContext* pContext)
{
	pContext->getInput()->guard(true);
	pMenu_->setState(nState);
	pMenu_->OnReset(pContext);
}


} // namespace Event end
} // namespace BMW end