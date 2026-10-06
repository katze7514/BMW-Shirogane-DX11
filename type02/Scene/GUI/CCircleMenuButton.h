/*
	katze 06/02/02
	サークルメニューで使われるボタン
*/
#pragma once

#include "../../mode.h"
#include "../../Movie/CMotionCircle.h"

namespace BMW{
namespace GUI{

class CCircleMenuButton : public Task::ITaskBase
{/**
	サークルメニューで使われるボタン
	位置の円運動をするだけ
 */
public:
	enum eState{
		NORMAL,
		MOTION,
	};
	// コンストラクタ・デストラクタ
	CCircleMenuButton():pTask_(new ITaskBase())
	{
		motion_.setEdging(100);
	}
	virtual ~CCircleMenuButton(){ DELETE_SAFE(pTask_); }

	// タスク
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	Task::ITaskBase*	getTask(){ return pTask_; }
	void				setTask(Task::ITaskBase* pTask)
	{
		pTask->setParent(smart_ptr<Task::ITaskBase>(this,false));
		DELETE_SAFE(pTask_);
		pTask_=pTask;
	}
	// 管理タスクゲットのヘルパ
	template<class T>
	T*	getTaskCast(){ return static_cast<T*>(pTask_); }

	// モーション設定のヘルパ
	// 角度は0～360で渡す
	void setCoreX(int nX){ motion_.setCoreX(nX); }
	void setCoreY(int nY){ motion_.setCoreY(nY); }
	void setR(int nR){ motion_.setR(nR); }
	void setStartAngle(int nAngle){ motion_.setStartAngle((nAngle*Draw::CDrawInfo::SCALE_ANGLE)/360); }
	void setEndAngle(int nAngle){ motion_.setEndAngle((nAngle*Draw::CDrawInfo::SCALE_ANGLE)/360); }
	void setStep(LONG lStep){ motion_.setStep(lStep); }

	// 操作
	const Draw::CDrawInfo getDrawInfo(bool bRela=true);
	void setDrawInfo(const Draw::CDrawInfo& drawInfo){ motion_.setCurrent(drawInfo); }

	void setX(int nX){ motion_.setX(nX); }
	void setY(int nY){ motion_.setY(nY); }
	void setAlpha(int nAlpha){ motion_.setAlpha(nAlpha); }
	void setWidth(LONG lWidth){ motion_.setWidth(lWidth); }
	void setHeight(LONG lHeight){ motion_.setHeight(lHeight); }
	void setAngle(int nAngle){ motion_.setAngle(nAngle); }

	// サイズの取得
	void getSize(LONG& lWidth,LONG& lHeight) const { pTask_->getSize(lWidth,lHeight);}
	void getDrawSize(LONG& lWidth,LONG& lHeight) const;

protected:
	Task::ITaskBase*		pTask_; // ボタン動作するはずのタスク
	Movie::CMotionCircle	motion_; // モーション
};

} // namespace GUI end
} // namespace BMW end