/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_bgm.h"

namespace BMW{
namespace Sound{
namespace Code{

void CCode_bgm::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case Ctrl::STOP: pContext->getBgmSound()->Stop();	break;
	
	case Ctrl::PLAY:
	{
		int nBgm=pContext->top();
		pContext->pop();
		pContext->getBgmSound()->change(nBgm);
		pContext->getBgmSound()->Play();
	}
	break;

	case Ctrl::PAUSE:	pContext->getBgmSound()->Pause();	break;
	case Ctrl::REPLAY:	pContext->getBgmSound()->RePlay();	break;

	case Ctrl::FADE_OUT:
	{
		int nFade=pContext->top();
		pContext->pop();
		pContext->getBgmSound()->FadeOut(nFade);
	}
	break;

	case Ctrl::FADE_IN:
	{
		int nFade=pContext->top();
		pContext->pop();
		int nBgm=pContext->top();
		pContext->pop();
		pContext->getBgmSound()->change(nBgm);
		pContext->getBgmSound()->FadeIn(nFade);
	}
	break;

	default: break;
	}

	// ó‹µ‚ð”½‰f‚·‚é‚½‚ß‚ÉƒtƒŒ[ƒ€‚ð‰ñ‚·
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
