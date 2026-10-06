#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Context/CSLGDef.h"
#include "../../Map/CMap.h"

#include "CCode_back.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_back::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nSymbol = p->top();
	p->pop();

	// ƒ}ƒbƒv”wŒiŽæ“¾
	Task::CTaskList* pBackList = p->getMap()->getBack();
	// ”wŒiƒNƒŠƒA
	pBackList->clearTask();
	// ”wŒi’Ç‰Á
	pBackList->addTask(p->getSLGDef().getEffect().createSymbol(nSymbol),0);
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end