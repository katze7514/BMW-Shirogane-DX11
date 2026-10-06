#include "stdafx.h"

#include "CDemoMovieClip.h"

namespace BMW{
namespace Demo{

void CDemoMovieClip::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END: break;
	default: if(IsEnd()) setState(END); break;
	}
}

} // namespace Demo end
} // namespace BMW end