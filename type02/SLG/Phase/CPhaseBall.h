/*
	katze 06/02/23
	ターンとかフェーズを表現する
*/
#pragma once

namespace BMW{
namespace SLG{

namespace Effect{
class CEffectMovieClip;
} // namespace Effect end

namespace Phase{

class CPhaseBall : public Task::ITaskBase
{/**
	ターンとかフェーズを表現する
 */
public:
	enum eState{
		NORMAL,
		EFFECT,
		EFFECT_END,
		MOVE,
	};
	// コンストラクタ・デストラクタ
	CPhaseBall():pBall_(NULL){ pButton_=new GUI::IButton(); }
	~CPhaseBall();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	
	// ロールオーバーで出すやつを更新
	void update(Task::CTaskContext*);
	// 出し入れ
	void intro(bool bIntro);

private:
	GUI::IButton* pButton_;
	GUI::CPanel* pBall_;
	GUI::CPanelCtrl* pTurn_;
	Effect::CEffectMovieClip* pEffect_;

	// 出し入れよう
	CInteriorCounter y_;
	int nEndY_;
};

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end