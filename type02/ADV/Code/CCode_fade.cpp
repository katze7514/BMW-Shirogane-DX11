#include "stdafx.h"

#include "CCode_fade.h"

namespace BMW{
namespace ADV{
namespace Code{

namespace{
// 白・黒・赤
const COLORREF rgb[]={ RGB(0,0,0),RGB(255,255,255), /*なぜかbの部分がr、rの部分がbと評価されている→*/RGB(0,0,255) };
}

void CCode_fade::OnAction(Task::CTaskContext* pContext)
{
	int nColor = pContext->top();
	pContext->pop();
	int nFrame = pContext->top();
	pContext->pop();
	int nCtrl = pContext->top();
	pContext->pop();

	Scene::CFoward* pForward = pContext->getApp()->getFoward();
	pForward->setFadeColor(rgb[nColor]);
	if(nCtrl==FADE_OUT)	pForward->fadeOut(nFrame);
	else				pForward->fadeIn(nFrame);

	// フレームを回す
	pContext->getTaskList()->killMe();
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end