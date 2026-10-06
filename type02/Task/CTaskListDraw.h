/*
	katze 06/02/02
	描画情報を持つタスクリスト
*/
#pragma once

#include "CTaskList.h"

namespace BMW{
namespace Task{

class CTaskListDraw : public CTaskList
{/**
	描画情報を持つタスクリスト
 */
public:
	// デストラクタ
	virtual ~CTaskListDraw(){}

	// 操作
	// デフォルトで親に対する相対値を返す
	// 必要無い時はfalseにすればいい
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela=true)
	{// 描画情報を計算して、結果を返す
		if(bRela&&(!getParent().isNull()))
			return getParent()->getDrawInfo().calcAbsolute(drawInfo_);
		else
			return drawInfo_;
	}
	virtual void setDrawInfo(const Draw::CDrawInfo& drawInfo){ drawInfo_=drawInfo; }

	virtual void setX(int nX){ drawInfo_.setX(nX); }
	virtual void setY(int nY){ drawInfo_.setY(nY); }
	virtual void setAlpha(int nAlpha){ drawInfo_.setAlpha(nAlpha); }
	virtual void setWidth(LONG lWidth){ drawInfo_.setWidth(lWidth); }
	virtual void setHeight(LONG lHeight){ drawInfo_.setHeight(lHeight); }
	virtual void setAngle(int nAngle){ drawInfo_.setAngle(nAngle); }

protected:
	Draw::CDrawInfo	drawInfo_;	// 描画に関係する情報
};

} // namespace Task end
} // namespace BMW end