#include "stdafx.h"

#include "CButtonKeep.h"

namespace BMW{
namespace GUI{

void CButtonKeep::OnAction(Task::CTaskContext* pContext)
{
	using BMW::Input::IInput;
	// カーソルがボタンの有効範囲内かどうか
	int nX,nY,nOK;
	bool bRange;
	// 入力取得
	pContext->getInput()->getCursolPos(nX,nY);
	nOK=pContext->getInput()->getInputState(IInput::OK);
	bRange=IsRange(nX,nY);

	switch(getState())
	{// ボタンの現在状態によって処理が分割できる
	case NORMAL:
		if(bRange)
		{// カーソルがボタン範囲に入ってきた
			actionOverIn(pContext);
		}
	break;

	case OVER:
		if(!bRange)
		{// カーソルがボタン範囲から出た
			actionOverOut(pContext);
		}
		else
		{
			if(nOK==IInput::PUSH)
			{// 決定ボタンが押された
				actionPress(pContext);
			}
		}
	break;

	case PRESS:
		if(!bRange)
		{// カーソルがボタン範囲から出た
			actionOverOut(pContext);
		}
		else
		{
			if(nOK==IInput::RELEASE)
			{// 決定ボタンが離された
				actionRelease(pContext);
			}
		}
	break;

	case PUSH:
		if(bRange)
		{// 範囲内で、
			if(nOK==IInput::RELEASE)
			{// 決定ボタンが押される
				actionCancel(pContext);
			}
		}
	break;

	default: break;
	}
}

void CButtonKeep::OnDraw(Task::CTaskContext* pContext)
{
	Draw::CDrawInfo info = getDrawInfo();
	// 描画
	(*pContext->getDrawPlane())->BltNatural(sprite_[getDrawState()].getPlane(),
											info.getX()-sprite_[getDrawState()].getX(), 
											info.getY()-sprite_[getDrawState()].getY(), 
											info.getAlpha(),
											NULL,
											&sprite_[getDrawState()].getRect());
}

////////////////////////////////////////////
// アクション
////////////////////////////////////////////
void CButtonKeep::actionRelease(Task::CTaskContext* pContext)
{
	CButton::actionRelease(pContext);
	// 押された状態へ
	setState(PUSH);
}

void CButtonKeep::actionCancel(Task::CTaskContext* pContext)
{
	pEvent_->setState(CANCEL);
	eventFun(pEvent_,pContext);
	// 押された状態をキャンセル
	actionOverIn(pContext);
}

} // namespace GUI end
} // namesapce BMW end