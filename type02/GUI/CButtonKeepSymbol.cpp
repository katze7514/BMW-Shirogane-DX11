#include "stdafx.h"

#include "CButtonKeepSymbol.h"

namespace BMW{
namespace GUI{

void CButtonKeepSymbol::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			OnAction(pContext);
			pSymbol_[getDrawState()]->Task(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{	pSymbol_[getDrawState()]->Task(pContext); }
	}
}

void CButtonKeepSymbol::OnAction(Task::CTaskContext* pContext)
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

__inline int CButtonKeepSymbol::getDrawState()
{
	switch(getState())
	{
	case OVER:
		return 1;

	case PRESS:
	case PUSH:
		return 2;

	default:
		return 0;
	}
}

void CButtonKeepSymbol::actionRelease(Task::CTaskContext* pContext)
{
	// 押された状態へ
	setState(PUSH);
	pEvent_->setState(RELEASE);
	eventFun(pEvent_,pContext);
	pSymbol_[PRESS]->OnReset(pContext);
}

void CButtonKeepSymbol::actionCancel(Task::CTaskContext* pContext)
{
	// 押された状態をキャンセル
	setState(OVER);
	pEvent_->setState(CANCEL);
	eventFun(pEvent_,pContext);
	actionOverIn(pContext);
}

} // namespace GUI end
} // namesapce BMW end