/*
	katze 07/01/28
	アイテム操作
*/
#pragma once

namespace BMW{
namespace ADV{
namespace API{

class CItem_ctrl : public Task::ITaskList
{/**
	アイテム・BP操作＆表示
 */
public:
	// 操作
	enum eCtrl{
		ADD,	// 追加
		DEL,	// 削除
	};
	// デストラクタ
	~CItem_ctrl();

	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	GUI::CPanel* pPanel_;

	void callTaskDraw(Task::CTaskContext*);

	// ライン設定
	void createLine(int nID, int nValue, int nLine);
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end