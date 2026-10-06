/**
	katze 06/07/22
	座標軸回転するキーフレーム
*/
#pragma once

#include "CKeyFrame.h"

namespace BMW{
namespace Movie{

class CKeyFrameBorn : public CKeyFrame
{/**
	座標軸回転するキーフレーム
*/
public:
	// デストラクタ
	virtual ~CKeyFrameBorn(){}

	// 操作
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela)
	{// ま、親は必ずいるし
		if(bRela)
			return getParent()->getDrawInfo().calcAbsoluteBorn(drawInfo_);
		else
			return drawInfo_;
	}
};

} // namespace Movie end
} // namespace BMW end