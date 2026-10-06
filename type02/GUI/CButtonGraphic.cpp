#include "stdafx.h"

#include "../Draw/CDrawInfo.h"
#include "../Task/CTaskContext.h"
#include "CButtonGraphic.h"

namespace BMW{
namespace GUI{
// usingéŒ¾
using Draw::CDrawInfo;

void CButtonGraphic::OnDraw(CTaskContext* pContext)
{// ƒ{ƒ^ƒ“‚Ìó‘Ô‚É‡‚í‚¹‚Ä•`‰æƒXƒvƒ‰ƒCƒg‚ªØ‚è‘Ö‚í‚é
	// •`‰æî•ñŽæ“¾
	CDrawInfo info = getDrawInfo();
	// •`‰æ
	(*pContext->getDrawPlane())->BltNatural(sprite_[getState()].getPlane(),
											info.getX()-sprite_[getState()].getX(), 
											info.getY()-sprite_[getState()].getY(), 
											info.getAlpha(),
											NULL,
											&sprite_[getState()].getRect());
}

} // namespace GUI end
} // namespace BMW end