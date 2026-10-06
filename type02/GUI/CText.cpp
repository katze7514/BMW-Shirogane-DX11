#include "stdafx.h"

#include "../Draw/CDrawInfo.h"
#include "../Task/CTaskContext.h"

#include "DB/IDGuiDef.h"

#include "CText.h"

namespace BMW{
namespace GUI{
// using宣言
using Draw::CDrawInfo;

CText::CText()
{// デフォルト設定
	// 影座標
	text_.GetFont()->SetShadowOffset(0,0);
	// ↓これにすると描画されない
	text_.GetFont()->SetBackColor(CLR_INVALID);
	// MS ゴシック ← 普通にデフォだった
	setFont(FONT_MINCHO);
	// 14pt
	setSize(14);
	// 黒
	setColor(RGB(63,57,54));
	// 寄せ位置
	setSide(Text::LEFT);
	// オフセットリセット
	nOffX_=0;
}

void CText::Task(CTaskContext* pContext)
{// どうせ描画するだけ
	if(!pContext->IsAction() && IsVisible())
		OnDraw(pContext);
}

void CText::OnDraw(CTaskContext* pContext)
{
	// 描画情報取得
	CDrawInfo info = getDrawInfo();

	// 描画
	(*pContext->getDrawPlane())->BltNatural(&text_,
											info.getX()-nOffX_,
											info.getY(),
											info.getAlpha());
}

void CText::UpdateText()
{
	text_.UpdateText();
	calcOffset();
}
void CText::UpdateTextA()
{ 
	text_.UpdateTextA();
	calcOffset(); 
}

void CText::UpdateTextAA()
{ 
	text_.UpdateTextAA();
	calcOffset(); 
}

void CText::calcOffset()
{
	switch(nSide_)
	{
	case Text::CENTER: // 中央寄せ
	{
		int nX, nY;
		text_.GetFont()->GetSize(nX,nY);
		nOffX_ = nX/2;
	}
	break;

	case Text::RIGHT: // 右寄せ
	{
		int nX, nY;
		text_.GetFont()->GetSize(nX,nY);
		nOffX_ = nX;
	}
	break;

	default: // 左寄せ
		nOffX_=0;
	break;
	}
}

void CText::setFont(int nFontID)
{ 
	text_.GetFont()->SetFont(nFontID);
	// フォントによってWeightをつける
	switch(nFontID)
	{
	case FONT_MINCHO:	// MS 明朝
	case FONT_P_MINCHO:	// MS P明朝
		getFontConf().SetWeight(700);
	break;

	default:
		getFontConf().SetWeight(300);
	break;
	}
}

} // namespace GUI end
} // namespace BMW end