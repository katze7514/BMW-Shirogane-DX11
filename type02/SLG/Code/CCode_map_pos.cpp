#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Map/CMap.h"

#include "CCode_map_pos.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_map_pos::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 移動先インデックスがスタックトップに
	int nY=p->top();
	p->pop();
	int nX=p->top();
	p->pop();

	p->getMap()->scrollPos(nX,nY);

	// 反映のためにフレームを回す
	p->getTaskList()->killMe();
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end