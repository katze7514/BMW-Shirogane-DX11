/*
	katze 06/07/22
	座標軸回転するトゥイーン
*/
#pragma once

#include "CTween.h"

namespace BMW{
namespace Movie{

class CTweenBorn : public CTween
{/**
	座標軸回転するトゥイーン
 */
public:
	virtual ~CTweenBorn(){}
	// 操作
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela)
	{// ま、親は必ずいるし
		if(bRela)
			return getParent()->getDrawInfo().calcAbsoluteBorn(motion_);
		else
			return motion_;
	}
};

} // namespace Movie end
} // namespace BMW end