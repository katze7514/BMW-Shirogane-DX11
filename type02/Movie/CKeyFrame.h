/*
	katze 05/05/10
	普通のキーフレームを表現するクラス
*/
#pragma once

#include "IKeyFrame.h"

namespace BMW{
namespace Movie{

class CKeyFrame : public IKeyFrame
{/**
	キーフレームを表現するクラス

	　また、Layerはこれを配列として持つことで表現される
	こいつが有効なフレーム数は、Stateで代用
	また、フレームNo.はTaskPriorityで代用
*/
public:
	// デストラクタ
	virtual ~CKeyFrame(){}

	// 操作
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela)
	{// ま、親は必ずいるし
		if(bRela)
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
	// 描画情報
	Draw::CDrawInfo drawInfo_;
};

} // namespace Movie end
} // namespace BMW end