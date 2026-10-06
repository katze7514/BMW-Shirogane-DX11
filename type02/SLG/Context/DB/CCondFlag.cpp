#include "stdafx.h"

#include "../CSLGContext.h"
#include "CCondFlag.h"

namespace BMW{
namespace SLG{

bool CCondFlag::judg(CSLGContext* p)
{
#ifdef BMW_DEBUG
	CDbg().Out("FLAG %d %d %d %d",getType(), getFlag(), getValue(), p->getValue(getFlag()));
#endif
	switch(getType())
	{ 
	case COND:
		return p->getValue(getFlag())==getValue();

	case COND_GLOBAL:
	{
		int n;
		p->getApp()->getExec().getFlag(getFlag(),n);
		#ifdef BMW_DEBUG
		CDbg().Out("COND_GLOBAL %d",n);
		#endif
		return n==getValue();
	}

	case SET:
		p->setValue(getValue(),getFlag());
		#ifdef BMW_DEBUG
			CDbg().Out("SET_FLAG %d", p->getValue(getFlag()));
		#endif
		return true;

	case CALC:
		p->setValue(p->getValue(getFlag())+getValue(),getFlag());
		return true;

	case SET_GLOBAL:
		p->getApp()->getExec().setFlag(getFlag(),getValue());
		return true;

	case CALC_GLOBAL:
		p->getApp()->getExec().calcFlag(getFlag(),getValue());
		return true;
	}

	return false;
}

} // namespace SLG end
} // namespace BMW end