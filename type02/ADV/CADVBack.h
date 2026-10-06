/*
	katze 05/05/18
	update 06/02/12
	ADVシーン背景
*/
#pragma once

namespace BMW{
namespace ADV{

class CADVBack : public Task::ITaskBase
{/**
	ADVシーン背景
 */
public:
	// デストラクタ
	virtual ~CADVBack();
	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);

	// 取得
	GUI::CGraphic*	getBack(){ return pBack_; }
	GUI::CText*		getBackName();

private:
	// 背景要素
	GUI::CGraphic*	pBack_;
	GUI::CPanel*	pBackName_;
};

} // namespace ADV end
} // namespace BMW end