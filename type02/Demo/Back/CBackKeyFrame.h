/*
	katze 06/04/03
	戦闘背景用キーフレーム
*/
#pragma once

#include "../../Movie/CKeyFrame.h"

namespace BMW{
namespace Demo{

class CBackKeyFrame : public Movie::CKeyFrame
{
public:
	// デストラクタ
	virtual ~CBackKeyFrame(){}

	virtual void setDrawInfo(const Draw::CDrawInfo& drawInfo)
	{
		drawInfo_.setX(drawInfo.getX()<<16);
		drawInfo_.setY(drawInfo.getY()<<16);
		drawInfo_.setAlpha(drawInfo.getAlpha());
		drawInfo_.setWidth(drawInfo.getWidth());
		drawInfo_.setHeight(drawInfo.getHeight());
		drawInfo_.setAngle(drawInfo.getAngle());
	}
};

} // namespace Demo end
} // namespace BMW end