/*
	katze 11/03/21
	’Š‘I” —”
*/
#include "stdafx.h"

#include "CRandLottery.h"

namespace katzeSDK{
namespace Math{

void CRandLottery::init()
{
	for(int i=1; i<100; ++i)
		aLottery_[i].init(i);
}

bool CRandLottery::lot(unsigned int rate)
{
	if(rate<=0)		return false;
	if(rate>=100)	return true;

	return aLottery_[rate].lot();
}

} // namespace Math end
} // namespace katzeSDK end
