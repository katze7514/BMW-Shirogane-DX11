#include "stdafx.h"

#include "IDemoBackLine.h"

namespace BMW{
namespace Demo{
// staticéŒ¾
int IDemoBackLine::nBackState_;
bool IDemoBackLine::bBackVisible_=true;
bool IDemoBackLine::bForwardVisible_=true;


void IDemoBackLine::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
			OnAction(pContext);
	}
	else
	{
		if(IsBackVisible() && IsVisible())
			OnDraw(pContext);
	}
}

void IDemoBackLine::setVelFloat(float fVel, int nIndex)
{ 
	// •‚“®¬”‚ğŒÅ’è¬”‚Ö•ÏŠ·
	// ®”•”
	int n = floor(fVel);
	// ¬”•”
	int f = (fVel-n) * (2<<16);
	// ŒÅ’è¬”‰»
	int nVel = (n<<16) + f;

	// ‘¬“x‚Æ‚µ‚Äİ’è
	nVel_[9-nIndex].setNum(nVel);
	nVel_[20-9+nIndex].setNum(-nVel);
}

} // namespace Demo end
} // namespace BMW end