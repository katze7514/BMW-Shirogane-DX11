#include "stdafx.h"

#include "../Back/IDemoBackLine.h"

#include "CCode_back_vel.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_back_vel::OnAction(Task::CTaskContext* pContext)
{
	IDemoBackLine::setBackState(getState());
}

} // namespace Code end
} // namespace Demo end
} // namespace BMW end