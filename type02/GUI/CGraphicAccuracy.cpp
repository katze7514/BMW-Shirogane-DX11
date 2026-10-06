#include "stdafx.h"

#include "CGraphicAccuracy.h"

namespace BMW{
namespace GUI{

void CGraphicAccuracy::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	Draw::CDrawInfo info = getDrawInfo();
	// •`‰æ
	(*pContext->getDrawPlane())->BltNatural(sprite_.getPlane(),
											roundRShift(info.getX(),16)-sprite_.getX(), 
											roundRShift(info.getY(),16)-sprite_.getY(), 
											info.getAlpha(),
											NULL,
											&sprite_.getRect());
}

} // namespace GUI end
} // namespace BMW end