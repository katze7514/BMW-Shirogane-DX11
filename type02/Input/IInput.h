/*
	katze 05/02/20
	入力のインターフェイスクラス
*/
#pragma once

namespace BMW{
namespace Input{

class IInput
{/**
	入力の抽象化を担うクラス
	こいつを介すことで、BMWシステム内では入力IDだけで
	入力を検知することになる
 */
public:
	typedef delegate<void> InputEvent;
	// 入力ID
	enum eInputID{
		OK,		// 決定
		CANCEL,	// キャンセル
		// 他にショートカットがあるなら追加する
		CTRL,
	};
	// 入力状態ID
	enum eInputStateID{
		NO,			// 何もされてない
		PRESS,		// 押された
		PUSH,		// 押されている
		RELEASE,	// 離された
		DRAG,		// ドラッグ状態
	};
	// コンストラクタ・デストラクタ
	virtual ~IInput(){}

	// 操作-----------------------------------------
	// 指定した入力IDの状態がを取得する
	virtual int	 getInputState(int nInput)=0;
	virtual void resetInputState()=0;

	// 入力のガード
	// ガードが成立しているとすべての入力がNOと認識される
	virtual bool IsGuard()const=0;
	virtual void guard(bool bGuard)=0;

	// カーソル位置の設定・取得
	virtual void getCursolPos(int& nX, int& nY)=0;
	virtual void setCursolPos(int nX, int nY)=0;
	// カーソル描画のオフセット値の設定・取得
	virtual void getCursolOffset(int& nX, int& nY)=0;
	virtual void setCursolOffset(int nX, int nY)=0;
	// カーソル取得のガード
	virtual bool IsGuardCursol()const=0;
	virtual void guardCursol(bool bGuard)=0;
	// カーソルの可視・非可視
	virtual bool IsCursolVisible()const=0;
	virtual void cursolVisible(bool bVisible)=0;
	// ドラッグ判定をするかしないか
	virtual bool IsGuardDrag()const=0;
	virtual void guardDrag(bool bGuard)=0;

	// アクション------------------------------------
	// 現在のカーソル位置から、(nX,nY)にnStepで移動する。終了するとfunが呼ばれる
	virtual void actionMove(int nX, int nY, int nStep, const InputEvent& fun)=0;
};

// よく使うやつをショートカットする関数
__inline bool releaseOK(Task::CTaskContext* pContext)
{
	return pContext->getInput()->getInputState(IInput::OK)==IInput::RELEASE;
}
__inline bool releaseCancel(Task::CTaskContext* pContext)
{	
	return pContext->getInput()->getInputState(IInput::CANCEL)==IInput::RELEASE;
}

} // namespace Input end
} // namespace BMW end
