#include "stdafx.h"

#include "CCode_movie_end.h"

namespace BMW{
namespace Movie{
namespace Code{

void CCode_movie_end::OnAction(Task::CTaskContext* pContext)
{
	// ‚±‚¢‚Â‚Ì“ñ‚Âã‚Ìe‚ÍAMovieClip
	getParent()->getParent()->setState(CMovieClip::END);
}

} // namespace Code end
} // namespace Movie end
} // namespace BMW end