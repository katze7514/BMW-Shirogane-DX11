#include "stdafx.h"

#include "../../Face/DB/CDataFace.h"
#include "../../Movie/DB/CSymbolDB.h"
#include "CGraphicName.h"

namespace BMW{
namespace GUI{

void CGraphicName::setCharaName(int nCharaID, Task::CTaskContext* pContext, int nX, int nY)
{
	// 名前グラフィック設定
	if(nCharaID<0)
	{// nCharaIDが負の時は？？？を表示する
		pContext->getApp()->getFoward()->getSymbolDB().setGraphicGui(this,"UNKNOWN_G",nX,nY);
	}
	else
	{// そうでなければ普通に設定
		pContext->getApp()->getFaceMap().setNameGraphic(this, nCharaID, nX, nY);
	}
	// 寄せに合わせてオフセット移動
	sprite_.setOffsetPos(getToward()==Face::CDataFace::LEFT ? 0 : sprite_.getWidth(), sprite_.getY());
}

} // namespace GUI end
} // namespace BMW end