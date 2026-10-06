/*
	katze 06/02/28
	養成の基底クラス
*/
#pragma once

namespace BMW{
namespace Inter{
namespace Chara{

class ITrainBase : public Task::ITaskBase
{/**
	養成の基底クラス
 */
public:
	typedef delegate<void,int,Task::CTaskContext*> TrainEvent;
	// デストラクタ
	virtual ~ITrainBase(){ DELETE_SAFE(pPanel_); }

	// タスク
	virtual void Task(Task::CTaskContext* pContext){ pPanel_->Task(pContext); }
	// 設定・取得
	void setEventHandler(const TrainEvent& fun){ fun_=fun; }
	
	// イベントハンドラ
	virtual void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*){}

protected:
	GUI::CPanel* pPanel_;

	// イベントハンドラ
	TrainEvent fun_;
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end