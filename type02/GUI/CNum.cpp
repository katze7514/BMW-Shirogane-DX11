#include "stdafx.h"

#include "../Draw/CDrawInfo.h"
#include "../Task/CTaskContext.h"
#include "CNum.h"

namespace BMW{
namespace GUI{
// using宣言
using Draw::CDrawInfo;

void CNum::Task(CTaskContext* pContext)
{// どうせ描画するだけさー
	if(!pContext->IsAction() && IsVisible())
		OnDraw(pContext);
}

void CNum::OnDraw(CTaskContext* pContext)
{
	// 描画情報を取得
	CDrawInfo info = getDrawInfo();

	// ループ最適化のためのループ内変数の局所化
	DWORD nAlpha = info.getAlpha();
	LONG lNum = getNum();
	bool bMinus=false;
	if(lNum<0){ bMinus=true; lNum=-lNum;}
	int nTargetNum=0;
	int nX = info.getX();
	int nY;
	int nBaseLine = info.getY();

	// 数字の表示
	// とりあえず、0一個は表示させるためにdo-while
	do
	{
		nTargetNum=lNum%10;
		lNum/=10;

		// 描画位置計算
		nX -= sprite_[nTargetNum].getWidth();
		// ベースラインを揃える
		nY = nBaseLine - sprite_[nTargetNum].getHeight();
	
		// 描画
		(*pContext->getDrawPlane())->BltNatural(sprite_[nTargetNum].getPlane(), 
												nX, 
												nY, 
												nAlpha, 
												NULL, 
												&(sprite_[nTargetNum].getRect()));
		// 数字の周り込みのために
		nX += sprite_[nTargetNum].getX();
	}while(lNum!=0);

	// 記号表示
	// 負だったら-を表示
	if(bMinus)
	{
		// 描画位置計算
		nX -= sprite_[MINUS].getWidth();
		// ベースラインを揃える
		nY = nBaseLine - sprite_[MINUS].getHeight();

		// 描画
		(*pContext->getDrawPlane())->BltNatural(sprite_[MINUS].getPlane(), 
												nX, 
												nY, 
												nAlpha, 
												NULL, 
												&(sprite_[MINUS].getRect()));
	}
	else if(IsPlus())
	{// +記号を描画するなら描画
		// 描画位置計算
		nX -= sprite_[PLUS].getWidth();
		// ベースラインを揃える
		nY = nBaseLine - sprite_[PLUS].getHeight();

		// 描画
		(*pContext->getDrawPlane())->BltNatural(sprite_[PLUS].getPlane(), 
												nX, 
												nY, 
												nAlpha, 
												NULL, 
												&(sprite_[PLUS].getRect()));
	}
}

void CNum::getSize(LONG& lWidth, LONG& lHeight) const
{
	LONG lNum = getNum();
	bool bMinus = lNum<0 ? true : false;
	int nTargetNum=0;
	int nSizeX=0;
	int nSizeY=0;
	int nY;

	// 数字の幅と高さ
	do
	{
		nTargetNum=lNum%10;
		lNum/=10;

		// 幅
		nSizeX += sprite_[nTargetNum].getWidth();
		// 数字の周り込みのために
		nSizeX -= sprite_[nTargetNum].getX();
		// 高さ
		nY = sprite_[nTargetNum].getHeight();
		if(nY>nSizeY) nSizeY=nY;
	}while(lNum!=0);

	// 記号表示
	// 負だったら-を表示するのでその分プラス
	if(bMinus)
	{
		// 描画位置計算
		nSizeX += sprite_[MINUS].getWidth();
		// ベースラインを揃える
		nY = sprite_[MINUS].getHeight();
		if(nY>nSizeY) nSizeY=nY;
	}
	else if(IsPlus())
	{// +記号を描画するならその分プラス
		// 描画位置計算
		nSizeX += sprite_[PLUS].getWidth();
		// ベースラインを揃える
		nY = sprite_[PLUS].getHeight();
		if(nY>nSizeY) nSizeY=nY;
	}

	// 最後に値を設定して終了
	lWidth=nSizeX;
	lHeight=nSizeY;
}

void CNum::getDrawSize(LONG& lWidth, LONG& lHeight) const
{
	getSize(lWidth,lHeight);
}

} // namespace GUI end
} // namespace BMW end