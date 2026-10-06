#include "stdafx.h"

#include "../Draw/CDrawInfo.h"

#include "CTaskContext.h"
#include "ITaskBase.h"

namespace BMW{
namespace Task{

void ITaskBase::Task(CTaskContext* pContext)
{// Task“®ì
	if(pContext->IsAction())
	{
		if(IsValid()) OnAction(pContext);
	}
	else
	{
		if(IsVisible()) OnDraw(pContext);	
	}
}

const Draw::CDrawInfo ITaskBase::getDrawInfo(bool bRela){ return Draw::CDrawInfo(); }

} // namespace Task
} // namespace BMW