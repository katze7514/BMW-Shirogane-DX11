#include "stdafx.h"

#include "CDemoBackLine.h"

namespace BMW{
namespace Demo{

CDemoBackLine::~CDemoBackLine()
{
	DELETE_SAFE(pSymbol_[0]);
	DELETE_SAFE(pSymbol_[1]);
}

void CDemoBackLine::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
			OnAction(pContext);
	}
	ef(IsVisible())
	{
		pSymbol_[0]->Task(pContext);
		pSymbol_[1]->Task(pContext);
	}
}

void CDemoBackLine::OnAction(Task::CTaskContext* pContext)
{
	nX_[0] = nX_[0] + nVel_[getBackState()];
	nX_[1] = nX_[1] + nVel_[getBackState()];
	
	if(nX_[0].getNum()<=point_[0].nLeft_)	nX_[0].setNum(point_[0].nMove_+nX_[1].getNum());
	ef(nX_[0].getNum()>=point_[0].nRight_)	nX_[0].setNum(-point_[0].nMove_+nX_[1].getNum());
	ef(nX_[1].getNum()<=point_[1].nLeft_)	nX_[1].setNum(point_[1].nMove_+nX_[0].getNum());
	ef(nX_[1].getNum()>=point_[1].nRight_)	nX_[1].setNum(-point_[1].nMove_+nX_[0].getNum());

	pSymbol_[0]->setX(nX_[0].getNum());
	pSymbol_[1]->setX(nX_[1].getNum());
}

} // namespace Demo end
} // namespace BMW end