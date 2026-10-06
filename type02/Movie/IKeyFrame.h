/*
	katze 06/02/01
	キーフレームを表現する基底クラス
*/
#pragma once

namespace BMW{
namespace Movie{

class IKeyFrame : public Task::ITaskBase
{/**
	キーフレームを表現する基底クラス

	　また、Layerはこれを配列として持つことで表現される
	こいつが有効なフレーム数は、Stateで代用
	また、フレームNo.はTaskPriorityで代用
*/
public:
	// コンストラクタ・デストラクタ
	IKeyFrame():bShare_(false){ setState(1); }
	virtual ~IKeyFrame(){ if(!IsShare()) DELETE_SAFE(pTask_); }

	// タスク
	virtual void Task(Task::CTaskContext* pContext)
	{
		pTask_->Task(pContext);
	}
	virtual void OnReset(Task::CTaskContext* pContext)
	{
		if(!IsShare())
		{ 
			pTask_->OnReset(pContext);
			pTask_->setParent(smart_ptr<Task::ITaskBase>(this,false));
		}
	}

	// 設定・取得
	Task::ITaskBase*	getTask(){ return pTask_; }
	void				setTask(Task::ITaskBase* pTask)
	{
		pTask_=pTask;
		if(pTask_!=NULL)
			pTask_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	}

	bool IsShare() const { return bShare_; }
	void share(bool bShare){ bShare_=bShare; }

	// keyframe自体が動作を持ってる時使う。
	// 単なるkeyframeは、常に動作が終了している
	// もちろん、保持してるタスクが終了してるかは別
	virtual bool	IsEnd(){ return true; }

	virtual void	getSize(LONG& lWidth, LONG& lHeight)const{ pTask_->getSize(lWidth,lHeight); }
	virtual void	getDrawSize(LONG& lWidth, LONG& lHeight)const{ getSize(lWidth,lHeight); }

protected:
	Task::ITaskBase*	pTask_;
	bool bShare_;
};

} // namespace Movie end
} // namespace BMW end