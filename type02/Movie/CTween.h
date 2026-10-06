/*
	katze 05/05/07
	トゥイーン
*/
#pragma once

#include "CMotion.h"
#include "IKeyFrame.h"

namespace BMW{
namespace Movie{

class CTween : public IKeyFrame
{/**
	トゥイーン
 */
public:
	// デストラクタ
	virtual ~CTween(){}
	// タスク
	void Task(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnDraw(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// 操作
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela)
	{// ま、親は必ずいるし
		if(bRela)
			return getParent()->getDrawInfo().calcAbsolute(motion_);
		else
			return motion_;
	}

	// 設定系はとりあえず現在値としていれるが、IncかDecか動いてしまえば
	// 計算し直される
	virtual void setDrawInfo(const Draw::CDrawInfo& drawInfo){ motion_.setCurrent(drawInfo); }

	virtual void setX(int nX){ motion_.setX(nX); }
	virtual void setY(int nY){ motion_.setY(nY); }
	virtual void setAlpha(int nAlpha){ motion_.setAlpha(nAlpha); }
	virtual void setWidth(LONG lWidth){ motion_.setWidth(lWidth); }
	virtual void setHeight(LONG lHeight){ motion_.setHeight(lHeight); }
	virtual void setAngle(int nAngle){ motion_.setAngle(nAngle); }

	// 設定・取得
	const CMotion&	getMotion() const { return motion_; }
	void			setMotion(const CMotion& motion){ motion_=motion; }

	// Tweenの場合は、CMotionが動作終了しているか？
	virtual bool	IsEnd(){ return motion_.IsEnd(); }

	void getDrawSize(LONG& lWidth, LONG& lHeight)const;

protected:
	CMotion	motion_;
};

} // namespace Movie end
} // namespace BMW end