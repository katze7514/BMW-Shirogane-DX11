#include "stdafx.h"

#include "../../Face/DB/CDataFace.h"
#include "CGraphicFace.h"

namespace BMW{
namespace GUI{

void CGraphicFace::setFace(int nCharaID, int nFaceID, Task::CTaskContext* pContext, int nX, int nY)
{
	// 顔スプライトゲットー
	pContext->getApp()->getFaceMap().setFaceGraphic(this,nCharaID,nFaceID,getToward(),nX,nY,IsBattle());
	// こいつをつかうと自動的にベースラインがそろえられる
	// 左向きだと左下。右向きだと右下
	sprite_.setOffsetPos(IsBattle() || getToward()==Face::CDataFace::LEFT? 0 : sprite_.getWidth(), 
						 sprite_.getHeight());
}

void CGraphicFace::setFace(int nCharaID, const string& sFaceID, Task::CTaskContext* pContext, int nX, int nY)
{
	// 顔スプライトゲットー
	pContext->getApp()->getFaceMap().setFaceGraphic(this,nCharaID,sFaceID,getToward(),nX,nY,IsBattle());
	// こいつをつかうと自動的にベースラインがそろえられる
	// 左向きだと左下。右向きだと右下
	sprite_.setOffsetPos(IsBattle() || getToward()==Face::CDataFace::LEFT ? 0 : sprite_.getWidth(), 
						 sprite_.getHeight());
}

} // namespace GUI end
} // namespace BMW end