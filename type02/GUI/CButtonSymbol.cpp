#include "stdafx.h"

#include "CButtonSymbol.h"

namespace BMW{
namespace GUI{

void CButtonSymbol::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			OnAction(pContext);
			pSymbol_[getState()]->Task(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{	pSymbol_[getState()]->Task(pContext); }
	}
}

void CButtonSymbol::OnReset(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pSymbol_[0]->OnReset(pContext);
	pSymbol_[1]->OnReset(pContext);
	pSymbol_[2]->OnReset(pContext);
}

void CButtonSymbol::actionOverIn(CTaskContext* pContext)
{
	CButton::actionOverIn(pContext);
	pSymbol_[OVER]->OnReset(pContext);
}

void CButtonSymbol::actionOverOut(CTaskContext* pContext)
{
	CButton::actionOverOut(pContext);
	pSymbol_[NORMAL]->OnReset(pContext);
}

void CButtonSymbol::actionPress(CTaskContext* pContext)
{
	CButton::actionPress(pContext);
	pSymbol_[PRESS]->OnReset(pContext);
}

void CButtonSymbol::actionRelease(CTaskContext* pContext)
{
	CButton::actionRelease(pContext);
	pSymbol_[OVER]->OnReset(pContext);
}

} // namespace GUI end
} // namespace BMW end