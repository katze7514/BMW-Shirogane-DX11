/*
	katze 05/02/18
	入力タスク
*/
#pragma once

#include "../GUI/CGraphic.h"
#include "../Task/ITaskBase.h"
#include "IInput.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Input{
using namespace Task;

class CTaskInput : public IInput, public ITaskBase
{/**
	入力の実装
	今回はカーソル描画も行う
	動作そのものは委譲スタイル。
 */
public:
	enum eState{
		NORMAL,
		MOVE,
	};
	// コンストラクタ・デストラクタ
	CTaskInput();
	~CTaskInput();

	// タスク処理
	void OnAction(CTaskContext*);
	void OnDraw(CTaskContext*);

	// 取得
	GUI::CGraphic*	getGraphic(){ return pMouse_; }
	CFixMouse&		getMouse(){ return mouse_; }
	CKeyInput&		getKeyBoard(){ return key_; }

	// 操作
	int	 getInputState(int nInput);
	void resetInputState();
	bool IsGuard()const{ return bGuard_; }
	void guard(bool bGuard){ bGuard_=bGuard; mouse_.ResetButton(); }

	void getCursolPos(int& nX, int& nY);
	void setCursolPos(int nX, int nY);
	void getCursolOffset(int& nX, int& nY){ nX=nOx_; nY=nOy_; }
	void setCursolOffset(int nX, int nY){ nOx_=nX; nOy_=nY; }
	bool IsGuardCursol()const{ return bGuardCursol_; }
	void guardCursol(bool bGuard){ bGuardCursol_=bGuard; }
	bool IsCursolVisible()const{ return IsVisible(); }
	void cursolVisible(bool bVisible){ visible(bVisible); }

	bool IsGuardDrag()const{ return bGuardDrag_; }
	void guardDrag(bool bGuard){ bGuardDrag_=bGuard; if(!bGuard){ nDragFlag_=NO_DRAG; nDragX_=-1; nDragY_=-1; } }
	int  getDragFlag(){ return nDragFlag_; }
	void setDragFlag(int nDragFlag){ nDragFlag_=nDragFlag; }

	void actionMove(int nX, int nY, int nStep, const InputEvent& fun);

private:
	// 入力実体
	CFixMouse mouse_;	// マウス
	CKeyInput key_;		// キーボード

	// 入力ガードフラグ
	bool bGuard_;
	// カーソルガードフラグ
	bool bGuardCursol_;

	// ドラッグガードフラグ
	bool bGuardDrag_;
	// ドラッグフラグ
	enum eDrag{
		NO_DRAG,
		READY_DRAG,
		FIX_DRAG,
	};
	int nDragFlag_;
	// ドラッグ判定用1フレーム前のカーソル位置
	int nDragX_,nDragY_;
	// ドラッグ判定
	void IsDrag();

	// マウスグラフィック
	GUI::CGraphic* pMouse_;
	// マウスグラフィックのオフセット
	int nOx_,nOy_;
	
	// カーソル移動用
	Movie::CMotion motion_;
	InputEvent fun_;
};

} // namespace Input end
} // namespace BMW end