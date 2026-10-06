/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_se.h"

namespace BMW{
namespace Sound{
namespace Code{

void CCode_se::OnAction(Task::CTaskContext* pContext)
{
	int nSe=pContext->top();
	pContext->pop();

	//CDbg().Out("%d %d",getState(),nSe);
	switch(getState())
	{
	case Ctrl::STOP:
		pContext->getApp()->getSeDB().Stop(nSe);
	break;

	case Ctrl::PLAY:
		pContext->getApp()->getSeDB().Play(nSe);
	break;

	case Ctrl::PLAY_L:
		pContext->getApp()->getSeDB().PlayL(nSe);
	break;

	case Ctrl::PLAY_N:
		pContext->getApp()->getSeDB().PlayN(nSe);
	break;

	case Ctrl::PLAY_LN:
		pContext->getApp()->getSeDB().PlayLN(nSe);
	break;

	default: break;
	}
	// Œ‹‰Ê”½‰f‚Ì‚½‚ß‚ÉƒtƒŒ[ƒ€‚ð‰ñ‚·
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
