#include "stdafx.h"

#include "CCode_movie_stop.h"

namespace BMW{
namespace Movie{
namespace Code{

void CCode_movie_stop::OnAction(Task::CTaskContext* pContext)
{
	// ‚±‚¢‚Â‚Ì“ñ‚Âã‚Ìe‚ÍAMovieClip
	getParent()->getParent()->setState(CMovieClip::STOP);
	getParent()->getParent()->valid(false);
}

} // namespace Code end
} // namespace Movie end
} // namespace BMW end