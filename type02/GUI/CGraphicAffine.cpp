#include "stdafx.h"

#include "CGraphicAffine.h"

namespace BMW{
namespace GUI{

void CGraphicAffine::OnDraw(CTaskContext* pContext)
{
	// GeneralMorphを直接呼ぶ形
	// 画像矩形をDrawInfoに併せて変形していく
	POINT aSrcPoint[4],aDstPoint[4];

	// モーフポイントを初期化
	const RECT& rect = sprite_.getRect();
	aSrcPoint[0].x = aDstPoint[0].x = rect.left;
	aSrcPoint[0].y = aDstPoint[0].y = rect.top;
	aSrcPoint[1].x = aDstPoint[1].x = rect.right-1;
	aSrcPoint[1].y = aDstPoint[1].y = rect.top;
	aSrcPoint[2].x = aDstPoint[2].x = rect.right-1;
	aSrcPoint[2].y = aDstPoint[2].y = rect.bottom-1;
	aSrcPoint[3].x = aDstPoint[3].x = rect.left;
	aSrcPoint[3].y = aDstPoint[3].y = rect.bottom-1;

	// アルファ
	DWORD nAlpha=Draw::CDrawInfo::SCALE_ALPHA;

	// 回転・拡縮中心
	int nOffX=0, nOffY=0;
	// オフセット（回転・拡縮中心）の初期化
	sprite_.getOffsetPos(nOffX,nOffY);
	for(unsigned int i=0; i<4; ++i)
	{
		aDstPoint[i].x -= nOffX;
		aDstPoint[i].y -= nOffY;
	}

	// 描画情報
	Draw::CDrawInfo drawInfo;
	Draw::CDrawInfo drawInfoParent;
	// 親だけど自分自身からスタート
	Task::ITaskBase* pParent = this;
	Task::ITaskBase* pParentParent = NULL;

	while(pParent!=NULL)
	{
		// 描画情報取得
		drawInfo = pParent->getDrawInfo(false);

		// アルファ
		nAlpha = nAlpha * drawInfo.getAlpha() / Draw::CDrawInfo::SCALE_ALPHA;


		// 親の座標空間に変換
		// オフセット（中心）を親空間へ

		// 親上での座標
		nOffX = drawInfo.getX();
		nOffY = drawInfo.getY();

		// 一つ上の親をさらに取得
		pParentParent = pParent->getParent().getPointer();
		if(pParentParent!=NULL)
		{// いたら描画位置を変換
			drawInfoParent = pParentParent->getDrawInfo(false);
			// 描画位置（オフセット）を計算
			// 拡縮
			nOffX = nOffX * drawInfoParent.getWidth() / Draw::CDrawInfo::SCALE_RATE;
			nOffY = nOffY * drawInfoParent.getHeight() / Draw::CDrawInfo::SCALE_RATE;
			// 回転
			const LONG nSin = gSinTable.Sin(drawInfoParent.getAngle());
			const LONG nCos = gSinTable.Cos(drawInfoParent.getAngle());
			nOffX = roundRShift(nOffX * nCos - nOffY * nSin,16);
			nOffY = roundRShift(nOffX * nSin + nOffY * nCos,16);
		}

		//// 矩形の座標を親空間へ
		//for(unsigned int i=0; i<4; ++i)
		//{
		//	aDstPoint[i].x += nOffX;
		//	aDstPoint[i].y += nOffY;
		//}
		//// 回転・拡縮中心原点に移動
		//for(unsigned int i=0; i<4; ++i)
		//{
		//	aDstPoint[i].x -= nOffX;
		//	aDstPoint[i].y -= nOffY;
		//}

		// 拡縮
		// 矩形
		for(unsigned int i=0; i<4; ++i)
		{
			aDstPoint[i].x = aDstPoint[i].x * drawInfo.getWidth() / Draw::CDrawInfo::SCALE_RATE;
			aDstPoint[i].y = aDstPoint[i].y * drawInfo.getHeight() / Draw::CDrawInfo::SCALE_RATE;
		}
		
		// 回転
		const LONG nSin = gSinTable.Sin(drawInfo.getAngle());
		const LONG nCos = gSinTable.Cos(drawInfo.getAngle());

		// 矩形
		for(unsigned int i=0; i<4; ++i)
		{
			const int ox = aDstPoint[i].x;
			const int oy = aDstPoint[i].y;
			aDstPoint[i].x = roundRShift(ox * nCos - oy * nSin,16);
			aDstPoint[i].y = roundRShift(ox * nSin + oy * nCos,16);
		}

		// 回転・拡縮中心から戻す
		for(unsigned int i=0; i<4; ++i)
		{
			aDstPoint[i].x += nOffX;
			aDstPoint[i].y += nOffY;
		}

		// 一個上る
		pParent =pParent->getParent().getPointer();
	}

	// 結果を設定
	CSurfaceInfo::CMorphInfo morphInfo;
	morphInfo.Init(aSrcPoint,aDstPoint,NULL,false,4);

	//　描画
	(*pContext->getDrawPlane())->GeneralMorph(nAlpha!=255
												? CSurfaceInfo::eSurfaceBlendBltFast : CSurfaceInfo::eSurfaceBltAlphaFast,
												sprite_.getPlane()->GetConstSurfaceInfo(), 
												&morphInfo, 
												nAlpha!=255 ? &nAlpha : NULL);
}

} // namespace GUI end
} // namespace BMW end