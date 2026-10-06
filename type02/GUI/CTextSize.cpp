#include "stdafx.h"

#include "../Draw/CDrawInfo.h"
#include "../Task/CTaskContext.h"

#include "DB/IDGuiDef.h"

#include "CTextSize.h"

namespace BMW{
namespace GUI{
// usingéŒ¾
using Draw::CDrawInfo;

void CTextSize::OnDraw(CTaskContext* pContext)
{
	// •`‰æî•ñŽæ“¾
	CDrawInfo info = getDrawInfo();

	// ƒTƒCƒYŒvŽZ
	SIZE size;
	size.cx = roundRShift(nSizeX_ * info.getWidth(),  8);
	size.cy = roundRShift(nSizeY_ * info.getHeight(), 8);
	// ƒIƒtƒZƒbƒg‚àŠg‘åEk¬‚µ‚È‚¢‚Æ•`‰æˆÊ’u‚ªãŽè‚­‡‚í‚È‚¢
	int nOx = roundRShift(nOffX_  * info.getWidth(),  8);
	
	// •`‰æ
	(*pContext->getDrawPlane())->BltNatural(&text_,
											info.getX()-nOx,
											info.getY(),
											info.getAlpha(),
											&size);
}

void CTextSize::UpdateText()
{
	CText::UpdateText();
	text_.GetFont()->GetSize(nSizeX_,nSizeY_);
}
void CTextSize::UpdateTextA()
{ 
	CText::UpdateTextA();
	text_.GetFont()->GetSize(nSizeX_,nSizeY_);
}

void CTextSize::UpdateTextAA()
{ 
	CText::UpdateTextAA();
	text_.GetFont()->GetSize(nSizeX_,nSizeY_);
}

} // namespace GUI end
} // namespace BMW end