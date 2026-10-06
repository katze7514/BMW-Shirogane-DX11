#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Map/CMap.h"

#include "CCode_map_scroll.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_map_scroll::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	int nIndex;
	if(p->top()==MAP)
	{
		p->pop();
		nIndex=p->top();
		p->pop();
	}
	else
	{// ƒLƒƒƒ‰ID‚È‚ç‚Î
		p->pop();
		nIndex=p->getCharaData(p->top())->getIndex();
		p->pop();
	}

	p->getMap()->scrollIndex(nIndex);

	// ”½‰f‚Ì‚½‚ß‚ÉƒtƒŒ[ƒ€‚ð‰ñ‚·
	p->getTaskList()->killMe();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end