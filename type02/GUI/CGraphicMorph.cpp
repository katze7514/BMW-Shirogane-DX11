#include "stdafx.h"

#include "CGraphicMorph.h"

namespace BMW{
namespace GUI{

void CGraphicMorph::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	Draw::CDrawInfo info = getDrawInfo();
	// •`‰æ
	// ‰æ‘œ’†S‰ñ“]
	int nX = info.getX() - (roundRShift(sprite_.getX() * info.getWidth(),8));
	int nY = info.getY() - (roundRShift(sprite_.getY() * info.getHeight(),8));
	(*pContext->getDrawPlane())->RotateBlendBltFast(sprite_.getPlane(),
													 nX, 
													 nY,
													 info.getAngle(),
													 info.getWidth()<<8,
													 info.getHeight()<<8,
													 sprite_.getX(),
													 sprite_.getY(),
													 info.getAlpha(),
													 &sprite_.getRect());
}

} // namespace GUI end
} // namespace BMW end