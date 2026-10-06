#include "stdafx.h"

#include "../Input/IInput.h"
#include "../Task/CTaskContext.h"
#include "../Draw/CDrawInfo.h"

#include "IButton.h"

namespace BMW{
namespace GUI{
// using宣言
using Input::IInput;
using Draw::CDrawInfo;

void IButton::OnAction(CTaskContext* pContext)
{// ボタンとしての基本的な動作を行う
	// カーソルがボタンの有効範囲内かどうか
	int nX,nY,nOK;
	bool bRange;
	// 入力取得
	IInput *pInput = pContext->getInput();
	pInput->getCursolPos(nX,nY);
	nOK=pInput->getInputState(IInput::OK);
	// レンジ判定は、カーソルが非表示の時・入力がガードされてる時は無条件で失敗
	bRange= pInput->IsCursolVisible() && !pInput->IsGuard() && IsRange(nX,nY);

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

	default: break;
	}
}

void IButton::actionOverIn(CTaskContext* pContext)
{
	setState(OVER);
	// ポップアップ表示
	pContext->getApp()->getFoward()->createPopUp(this,pContext);
}

void IButton::actionOverOut(CTaskContext* pContext)
{
	setState(NORMAL);
}

void IButton::actionPress(CTaskContext* pContext)
{
	setState(PRESS);
}

void IButton::actionRelease(CTaskContext* pContext)
{
	setState(OVER);
}

bool IButton::IsRange(int nX, int nY)
{
	// 座標が負だったら、false
	if(nX<0 || nY<0) return false;

	CDrawInfo info = getDrawInfo();

	return
		info.getX()+range_.left<=nX&&info.getX()+range_.right>nX
		&&info.getY()+range_.top<=nY&&info.getY()+range_.bottom>nY
		? true : false;
}

void IButton::setButtonField(GUI::IButton* pButton, const RECT& rect, const string& sPopUp)
{
	// 領域指定
	pButton->setRange(rect);
	// ポップアップ
	pButton->setPopUp(sPopUp);
}

} // namespace GUI end
} // namespace BMW end