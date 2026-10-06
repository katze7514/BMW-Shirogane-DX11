#include "stdafx.h"

#include "CPopUpBase.h"

namespace BMW{
namespace Draw{

CPopUpBase::CPopUpBase()
{
}

CPopUpBase::CPopUpBase(const string& s)
{
	CPopUpBase();
	LPSTR pStr = const_cast<LPSTR>(s.c_str());
	CLineParser::ConvertCR(pStr);
	text_.setText(string(pStr));
}

////////////////////////////////////////////////
// タスク
///////////////////////////////////////////////
void CPopUpBase::OnReset(Task::CTaskContext* pContext)
{
	if(text_.getText().empty())
	{// テキストが設定されてなかったら、無意味なので
	 // 非動作
		valid(false);
		visible(false);
		return;
	}

	valid(true);
	visible(true);
	// フェードインしてくる
	setState(WAIT);
	// フェードインカウンタリセット
	counter_.Set(0,255,5);
	nFrame_=0;
	// 自分自身も見えなくなる
	setAlpha(0);
}

void CPopUpBase::OnAction(Task::CTaskContext* pContext)
{
	int nX,nY;
	pContext->getInput()->getCursolPos(nX,nY);
	if(!getButtonTask()->IsRange(nX,nY))
	{// カーソルが動いたら親リストからはずれる
		pContext->getTaskList()->removeMe();
	}
	ef(getState()==WAIT)
	{
		if(++nFrame_>10)
		{ 
			setPos(pContext);
			setState(FADE_IN);
		}
	}
	ef(getState()==FADE_IN)
	{
		setAlpha(++counter_);
		if(counter_.IsEnd()) setState(NORMAL);
	}
}

void CPopUpBase::OnDraw(Task::CTaskContext* pContext)
{
	// 描画
	// 枠
	(*pContext->getDrawPlane())->BlendBltFast(&rect_,
											  drawInfo_.getX(), 
											  drawInfo_.getY(), 
											  drawInfo_.getAlpha());

	// で、枠に文字を描画しちゃうで
	//CDbg().Out((*pContext->getDrawPlane())->BltAlphaFast(&text_.getTextPlane(),
	//									  drawInfo_.getX()+4, 
	//									  drawInfo_.getY()+5, 
	//									  drawInfo_.getAlpha()));
}

void CPopUpBase::getSize(LONG &lWidth, LONG& lHeight)
{
	int nX,nY;
	rect_.GetSize(nX,nY);
	lWidth=nX; lHeight=nY;
}

////////////////////////////////////////////////
// データ構築
////////////////////////////////////////////////
void CPopUpBase::createPopUp()
{
	// まずは、テキストプレーン生成！
	text_.setSize(12);
	text_.setFont(GUI::CText::FONT_GOTHIC);
	text_.setColor(RGB(4,4,4));
	text_.getFont()->SetHeight(14);
	text_.UpdateText();
	// テキスト描画の幅と高さゲット
	LONG lWidth,lHeight;
	text_.getSize(lWidth,lHeight);
	
	lWidth+=8;
	lHeight+=8;
	// 背景枠の構築
	rect_.CreateSurface(lWidth,lHeight,false);

	// CFastPlane::GetDC()はDX11モードでNULLを返すため、
	// 常時HDCを提供できるCDIBitmapにGDIで描画してからrect_へコピーする
	// (テキスト描画 CTextFastPlane::UpdateText と同じ実績パターン)
	CDIBitmap dib;
	dib.CreateSurface(lWidth,lHeight,32);
	HDC hdc = dib.GetDC();

	HGDIOBJ hPen, hOldPen;
    HGDIOBJ hBrush, hOldBrush;

    hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    hOldPen = SelectObject(hdc, hPen);
    hBrush = CreateSolidBrush(RGB(248,241,196));
    hOldBrush = SelectObject(hdc, hBrush);
    Rectangle(hdc, 0, 0, lWidth, lHeight);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);

	// CDIBitmapの内容をrect_へコピー
	CSurfaceInfo::CBltInfo info;
	rect_.GetSurfaceInfo()->GeneralBlt(CSurfaceInfo::eSurfaceBltFast,
		dib.GetSurfaceInfo(), &info);

	rect_.BltNatural(&text_.getTextPlane(),4,5);
}

void CPopUpBase::setPos(Task::CTaskContext* pContext)
{// 座標設定
	// 現在のカーソル位置に出現する
	int nX,nY;
	pContext->getInput()->getCursolPos(nX,nY);
	nX+=20;
	nY-=20;
	// ポップアップの大きさによって表示位置をずらす
	int nWidth,nHeight;
	rect_.GetSize(nWidth,nHeight);
	nY-=nHeight;
	// 横幅越えてたら、その分ずらす
	if(nX+nWidth > BMW::WINDOW_WIDTH)
		setX(nX - (nX+nWidth) + BMW::WINDOW_WIDTH);
	else
		setX(nX);

	// 縦幅越えてたら、その分ずらす
	if(nY < 0)
		setY(0);
	else
		setY(nY);
}

} // namespace Draw end
} // namespace BMW end