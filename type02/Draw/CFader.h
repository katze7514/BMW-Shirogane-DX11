/*
	katze 05/05/24
	フェーダー
*/
#pragma once

namespace BMW{
namespace Draw{

class CFader  : public Task::CTaskBase
{/**
	フェーダー

	ようは、ブラックインとか、ホワイトアウトとか
	そういうことをやるクラス
 */
public:
	typedef delegate<void,Task::CTaskContext*> FaderEvent;
	enum eState{
		NORMAL,
		FADE,
	};

	enum eFadeType{
		FADE_IN,
		FADE_OUT,
	};
	// コンストラクタ
	CFader();
	// タスク
	void OnAction(Task::CTaskContext*);
	void OnDraw(Task::CTaskContext*);

	// 設定・取得
	void setColor(COLORREF rgb){ plane_.SetFillColor(rgb); plane_.Clear(); }
	void fadeIn(int nFrame);
	void fadeOut(int nFrame);
	int	 getFadeType()const{ return nFadeType_; }
	void setFaderHandler(const FaderEvent& fun){ faderFun=fun; }

private:
	CFastPlane		 plane_;
	CInteriorCounter counter_;
	int nFadeType_; // 現在のフェード
	// 終了時に呼ばれるイベントハンドラ
	FaderEvent			faderFun;
};

} // namespace Draw end
} // namespace BMW end