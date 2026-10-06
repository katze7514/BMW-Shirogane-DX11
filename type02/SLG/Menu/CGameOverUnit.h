/**
	katze 06/07/20
	ゲームーオーバーダイアログ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Menu{

class CGameOverUnit : public Task::ITaskBase
{/**
	ゲームーオーバーダイアログ
 */
public:
	enum eState{
		INTRO,
		NORMAL,
	};
	enum eEvent{
		SAVE,MAP,TITLE
	};
	// デストラクタ
	virtual ~CGameOverUnit();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 設定
	void setInit(const GUI::CButton::ButtonEvent& fun, Task::CTaskContext* pContext);
	// イベントハンドラ
	void eventCursol();

private:
	GUI::CPanel*		pPanel_;
	CFastPlane*	plane_;
	CInteriorCounter	c_;
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end