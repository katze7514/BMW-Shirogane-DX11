#include "stdafx.h"

#include "CGraphicRotate.h"

namespace BMW{
namespace GUI{

void CGraphicRotate::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	Draw::CDrawInfo info = getDrawInfo();
	//CDbg().Out("%d %d",info.getAngle(),getParent()->getDrawInfo(false).getAngle());
	// •`‰æ
	// ‰æ‘œ’†S‰ñ“]
	// Šgk‚·‚éŽž‚ÍAWidth‚Ì‚ªÌ—p‚³‚ê‚é
	int nX = info.getX() - (roundRShift(sprite_.getX() * info.getWidth(),8));
	int nY = info.getY() - (roundRShift(sprite_.getY() * info.getWidth(),8));
	(*pContext->getDrawPlane())->RotateAlphaBltFast(sprite_.getPlane(),
													nX, 
													nY,
													info.getAngle(),
													info.getWidth()<<8,
													4,
													&sprite_.getRect());
}

} // namespace GUI end
} // namespace BMW end