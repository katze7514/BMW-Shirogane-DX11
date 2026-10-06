/*
	katze 06/07/22
	À•WŽ²‰ñ“]‚·‚éMovieClip
*/
#pragma once

#include "CMovieClip.h"

namespace BMW{
namespace Movie{

class CMovieClipBorn : public CMovieClip
{/**
	MovieClip
 */
public:
	virtual ~CMovieClipBorn(){}
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela=true)
	{// •`‰æî•ñ‚ðŒvŽZ‚µ‚ÄAŒ‹‰Ê‚ð•Ô‚·
		if(bRela&&(!getParent().isNull()))
			return getParent()->getDrawInfo().calcAbsoluteBorn(drawInfo_);
		else
			return drawInfo_;
	}
};

} // namespace Moive end
} // namespace BMW end