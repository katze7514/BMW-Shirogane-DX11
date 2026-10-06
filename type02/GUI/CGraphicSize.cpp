#include "stdafx.h"

#include "../Task/CTaskContext.h"
#include "../Draw/CDrawInfo.h"
#include "CGraphicSize.h"

namespace BMW{
namespace GUI{
// usingéŒ¾
using Draw::CDrawInfo;

void CGraphicSize::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	CDrawInfo info = getDrawInfo();
	// ƒTƒCƒYŒvŽZ
	SIZE size;
	size.cx = roundRShift(sprite_.getWidth()  * info.getWidth(),  8);
	size.cy = roundRShift(sprite_.getHeight() * info.getHeight(), 8);
	// ƒIƒtƒZƒbƒg‚àŠg‘åEk¬‚µ‚È‚¢‚Æ•`‰æˆÊ’u‚ªãŽè‚­‡‚í‚È‚¢
	int nOx = roundRShift(sprite_.getX()  * info.getWidth(),  8);
	int nOy = roundRShift(sprite_.getY()  * info.getHeight(), 8);

	// •`‰æ
	(*pContext->getDrawPlane())->BltNatural(sprite_.getPlane(),
											info.getX()-nOx, 
											info.getY()-nOy, 
											info.getAlpha(),
											&size,
											&sprite_.getRect());
}

void CGraphicSize::getDrawSize(LONG& lWidth,LONG& lHeight) const
{
	CGraphicSize* p = const_cast<CGraphicSize*>(this);
	CDrawInfo info = p->getDrawInfo();
	lWidth  = roundRShift(sprite_.getWidth()  * info.getWidth(),  8);
	lHeight = roundRShift(sprite_.getHeight() * info.getHeight(), 8);
}

} // namespace GUI end
} // namespace BMW end