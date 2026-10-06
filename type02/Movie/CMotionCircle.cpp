#include "stdafx.h"

#include "CMotionCircle.h"

namespace BMW{
namespace Movie{

bool CMotionCircle::inc()
{
	if(lCurrentStep_>=lStep_)
	{
		lCurrentAngle_=lEndAngle_;
		return true;
	}
	lCurrentStep_++;
	edging();
	calc();
	return false;
}

bool CMotionCircle::dec()
{
	if(lCurrentStep_<=0)
	{
		lCurrentAngle_=lStartAngle_;
		return true;
	}
	lCurrentStep_--;
	edging();
	calc();
	return false;
}

const double PI = 3.1415926535897932384626433832795;

void CMotionCircle::edging()
{// Šp‰Á‘¬“xì‚Á‚Ä‚éŠ´‚¶‚Ë
	LONG c,e;
	c = (lCurrentStep_<<16) / lStep_;
	e = gSinTable.Sin(roundRShift(c,8)) * nEdging_ / (100*PI);
	lCurrentAngle_ = (lStartAngle_<<16) + (lEndAngle_-lStartAngle_) * (c+e);
	lCurrentAngle_ = roundRShift(lCurrentAngle_,16);
}

void CMotionCircle::calc()
{// Œ»ÝŠp“x‚©‚çAÀ•WŒvŽZ
	current_.setX(gSinTable.Cos(lCurrentAngle_,nR_) + nX_);
	current_.setY(gSinTable.Sin(lCurrentAngle_,nR_) + nY_);
}

} // namespace Movie end
} // namespace BMW end