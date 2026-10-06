#include "stdafx.h"

#include "../Task/CTaskContext.h"
#include "../Draw/CDrawInfo.h"
#include "CGraphic.h"

namespace BMW{
namespace GUI{
// usingéŒ¾
using Draw::CDrawInfo;

void CGraphic::Task(CTaskContext* pContext)
{// ‚Ç‚¤‚¹•`‰æ‚·‚é‚¾‚¯
	if(!pContext->IsAction() && IsVisible())
		OnDraw(pContext);
}

void CGraphic::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	CDrawInfo info = getDrawInfo();
	// •`‰æ
	(*pContext->getDrawPlane())->BltNatural(sprite_.getPlane(),
											info.getX()-sprite_.getX(), 
											info.getY()-sprite_.getY(), 
											info.getAlpha(),
											NULL,
											&sprite_.getRect());
}

} // namespace GUI end
} // namespace BMW end