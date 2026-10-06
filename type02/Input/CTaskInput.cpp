#include "stdafx.h"

#include "../CApp.h"
#include "../Task/CTaskContext.h"
#include "../GUI/CGraphic.h"
#include "CTaskInput.h"

namespace BMW{
namespace Input{

CTaskInput::CTaskInput():bGuard_(false),bGuardCursol_(false),bGuardDrag_(true),nOx_(0),nOy_(0)
{
	// グラフィックの生成
	pMouse_=new GUI::CGraphic();
	// 自身を親として設定
	pMouse_->setParent(smart_ptr<ITaskBase>(this,false));
}

CTaskInput::~CTaskInput()
{
	 DELETE_SAFE(pMouse_);
}

void CTaskInput::OnAction(CTaskContext* pContext)
{
	// マウス系
	if(getState()==NORMAL)
	{
		// 入力をFlushするだけ
		if(!IsGuardCursol())
		{// カーソル移動がガードされてたら何もしない
			mouse_.Flush();
			// ドラッグ判定
			if(!IsGuardDrag()) IsDrag();
		}
	}
	else
	{// カーソル移動
		motion_.inc();
		setCursolPos(motion_.getX(),motion_.getY());
		if(motion_.IsEnd())
		{// カーソル移動終了
			setState(NORMAL);
			fun_(); // 終了時コールバック
		}
	}

	// キーボード系
	key_.Input();

	if(pContext->getApp()->getFoward()->IsFadeEnd())
	{// フェード中はできない
		// 強制終了
		if(key_.IsKeyPush(DIK_ESCAPE))
			pContext->getApp()->end();

		// ウインドウモードの切り替え
		if(key_.IsKeyPush(DIK_F4))
		{ 
			smart_ptr<CApp>& pApp = pContext->getApp();
			// 切り替えるとポップアップもリストアしないといけない
			pApp->getFoward()->clearPopUpAll();
			pApp->getGlobal().full(!pApp->GetDraw()->IsFullScreen());

			if(!pApp->GetDraw()->IsFullScreen())
				pApp->GetDraw()->SetDisplay(true,0,0,pApp->getGlobal().getColor());
			else
				pApp->GetDraw()->SetDisplay(false);

			CAppManager::GetMyWindow()->ShowCursor(false);
		}
	}

	// FPSの切り替え
	if(key_.IsKeyPush(DIK_F3))
		pContext->getApp()->getTimer()->SetFPS(30);
	ef(key_.IsKeyPush(DIK_F6))
		pContext->getApp()->getTimer()->SetFPS(60);

#ifdef BMW_DEBUG
	if(key_.IsKeyPush(DIK_F1))
		pContext->getApp()->getTimer()->SetFPS(10);
	ef(key_.IsKeyPush(DIK_F2))
		pContext->getApp()->getTimer()->SetFPS(20);
	ef(key_.IsKeyPush(DIK_F10))
		pContext->getApp()->getTimer()->SetFPS(100);
	ef(key_.IsKeyPush(DIK_F11))
		pContext->getApp()->getTimer()->SetFPS(1);
	ef(key_.IsKeyPush(DIK_F12))
		CApp::bAction_ = !CApp::bAction_;
#endif
}

void CTaskInput::OnDraw(CTaskContext* pContext)
{// マウスグラフィックを描画してやるだけ
	// マウスの現在位置を描画位置として設定する
	int nX,nY;
	getCursolPos(nX,nY);
	pMouse_->setX(nX+nOx_);
	pMouse_->setY(nY+nOy_);
	// 委譲するだけだけどね
	pMouse_->OnDraw(pContext);
}

int CTaskInput::getInputState(int nInput)
{
	// ガード中だったら、何も押されてない扱い
	if(IsGuard()) return NO;

	switch(nInput)
	{
	case OK: // 決定
		if(getDragFlag()==FIX_DRAG
		&& mouse_.LButton())			return DRAG;
		ef(mouse_.IsPushUpLButton())	return RELEASE;
		ef(mouse_.IsPushLButton())		return PUSH;
		ef(mouse_.LButton())			return PRESS;
		else return NO;

	case CANCEL: // キャンセル
		if(getDragFlag()==FIX_DRAG
		&& mouse_.RButton())			return DRAG;
		ef(mouse_.IsPushUpRButton())	return RELEASE;
		ef(mouse_.IsPushRButton())		return PUSH;
		ef(mouse_.RButton())			return PRESS;
		else return NO;

	case CTRL: // CTRLキー
		if(key_.IsKeyPush(DIK_LCONTROL)
		|| key_.IsKeyPush(DIK_RCONTROL))	return PUSH;
		ef(key_.IsKeyPress(DIK_LCONTROL)
		|| key_.IsKeyPress(DIK_RCONTROL))	return PRESS;
		else return NO;

	default:
		return NO;
	}
}

void CTaskInput::resetInputState()
{
	mouse_.ResetButton();
}

__inline void CTaskInput::getCursolPos(int& nX, int& nY)
{
	mouse_.GetXY(nX,nY);
}

__inline void CTaskInput::setCursolPos(int nX, int nY)
{
	mouse_.SetXY(nX,nY);
}

void CTaskInput::IsDrag()
{
	// ドラッグ判定
	int nX,nY;
	switch(getDragFlag())
	{
	case NO_DRAG: // ボタンが押されてたら、ドラッグレディへ
		if(mouse_.LButton() || mouse_.RButton())
		{
			setDragFlag(READY_DRAG);
			// データ更新
			getCursolPos(nX,nY);
			nDragX_=nX;
			nDragY_=nY;
		}
	break;

	case READY_DRAG: // 押された位置から、適当な幅動いたらドラッグモードへ
		if(!(mouse_.LButton() || mouse_.RButton()))
		{// ボタンが離されたら、初期フラグへ
			setDragFlag(NO_DRAG);
		}
		else
		{// 押されているなら、ドラッグモードに行くか判定
			getCursolPos(nX,nY);
			// 多少のブレを吸収するために、幅を持たせる
			if(abs(nX-nDragX_)>=4
			|| abs(nY-nDragY_)>=4)
			// ドラッグモード！
				setDragFlag(FIX_DRAG);
		}
	break;

	case FIX_DRAG: // ボタンを離したらドラッグ解除
		if(!(mouse_.LButton() || mouse_.RButton()))
		{
			setDragFlag(NO_DRAG);
			mouse_.ResetButton();
		}
	break;

	default: break;
	}
}


void CTaskInput::actionMove(int nX, int nY, int nStep, const InputEvent& fun)
{
	int noX,noY;
	getCursolPos(noX,noY);
	motion_.setStart(noX,noY);
	motion_.setEnd(nX,nY);
	motion_.setStep(nStep);
	motion_.reset();
	fun_=fun;
	setState(MOVE);
}

} // namespace Input end
} // namespace BMW end