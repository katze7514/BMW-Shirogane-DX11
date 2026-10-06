/*
	katze 05/07/24
	update 06/07/09
	セーブルール
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Save{

class CSave_rule : public Task::ITaskList
{/**
	セーブルール
 */
public:
	enum eState{
		INTRO,
		NORMAL,
		END,
	};
	enum eValue{
		SAVE,
		LOAD,
	};
	// デストラクタ
	~CSave_rule();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	
	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);
	void eventCancel(Task::CTaskContext*);

private:
	GUI::CPanel*		pPanel_;
	CInteriorCounter	c_;
	CFastPlane*			plane_;

	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);
};

} // namespace Save end
} // namespace SLG end
} // namespace BMW end