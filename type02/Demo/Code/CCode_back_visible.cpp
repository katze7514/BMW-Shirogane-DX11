#include "stdafx.h"

#include "../Back/IDemoBackLine.h"

#include "CCode_back_visible.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_back_visible::OnAction(Task::CTaskContext* pContext)
{
	if(getType()==BACK)
	{
		IDemoBackLine::backVisible(getState()==1);
	}
	ef(getType()==FORWARD)
	{
		IDemoBackLine::forwardVisible(getState()==1);
	}
	else
	{
		IDemoBackLine::backVisible(getState()==1);
		IDemoBackLine::forwardVisible(getState()==1);
	}
	
}

} // namespace Code end
} // namespace Demo end
} // namespace BMW end