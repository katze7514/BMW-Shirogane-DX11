#include "stdafx.h"

#include "CMotion.h"

namespace BMW{
namespace Movie{

const double PI = 3.1415926535897932384626433832795;
	
void CMotion::edging()
{
	LONG c	= (lCurrentStep_<<16) / lStep_;
	LONG e	= gSinTable.Sin(roundRShift(c,8)) * nEdging_ / (100*PI);
	current_= (start_<<16) + (end_-start_) * (c+e);
	current_.bitShiftR(16);
}

} // namespace Movie end
} // namespace BMW end