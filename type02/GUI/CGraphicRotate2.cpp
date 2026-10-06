#include "stdafx.h"

#include "CGraphicRotate2.h"

namespace BMW{
namespace GUI{

void CGraphicRotate2::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	Draw::CDrawInfo info = getDrawInfo();
	// •`‰æ
	// ‰æ‘œ’†S‰ñ“]
	(*pContext->getDrawPlane())->RotateAlphaBltFast2(sprite_.getPlane(),
													info.getX(), 
													info.getY(),
													info.getAngle(),
													info.getWidth()<<8,
													info.getHeight()<<8,
													4,
													&sprite_.getRect());
}

} // namespace GUI end
} // namespace BMW end