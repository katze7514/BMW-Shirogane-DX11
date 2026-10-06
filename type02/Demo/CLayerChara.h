/*
	katze 05/05/07
	デモ用キャラムービーレイヤー
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
class ITaskBase;
} // namespace Task end

namespace Demo{

class CLayerChara : public Task::ITaskBase
{/**
	デモ用キャラムービーレイヤー
 */
public:
	typedef vector<Task::ITaskBase*> task_vec;
	// コンストラクタ・デストラクタ
	CLayerChara():bEnd_(false){}
	virtual ~CLayerChara(){ clearTask(); }

	// タスク
	void Task(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// 操作
	void addTask(Task::ITaskBase* pBase);
	void clearTask();
	ITaskBase*	getTask(int nPri){ return vecTask_[nPri]; }

	bool IsEnd() const { return bEnd_;}
	int  getFrameSize() { return (int)vecTask_.size(); }

private:
	// こいつに、順番にムービークリップを並べる
	task_vec vecTask_;
	bool bEnd_;
};

} // namespace Demo end
} // namespace BMW end