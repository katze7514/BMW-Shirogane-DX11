#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "CMap.h"

#include "CMap_scroll.h"

namespace BMW{
namespace SLG{
namespace Map{

void CMap_scroll::OnInit(Task::CTaskContext* pContext)
{
	// マップ取得
	pMap_ = static_cast<CSLGContext*>(pContext)->getMap().getPointer();

	// 現在位置ゲット
	int nMapX,nMapY;
	pMap_->getMapPos(nMapX,nMapY);

	// 現在位置で初期化
	motion_.setStart(nMapX,nMapY);

	// 移動先情報取得
	int x = pContext->top();
	pContext->pop();
	int y = pContext->top();
	pContext->pop();
	// 値がINT_MAXだったらその方向は動かさない
	motion_.setEnd(x==INT_MAX ? nMapX : x, y==INT_MAX ? nMapY : y);

	motion_.setStep(pContext->top());
	pContext->pop();
	motion_.setEdging(pContext->top());
	pContext->pop();

	// 初期設定
	motion_.reset();
}

void CMap_scroll::OnAction(Task::CTaskContext* pContext)
{
	motion_.inc();
	pMap_->setX(motion_.getX());
	pMap_->setY(motion_.getY());
	if(motion_.IsEnd())	getTaskListCtrl()->returnTaskList();
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end