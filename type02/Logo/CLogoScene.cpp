#include "stdafx.h"

#include "../mode.h"
#include "../Scene/IDScene.h"
#include "CLogoScene.h"

namespace BMW{
namespace Logo{

void CLogoScene::OnInit(Task::CTaskContext* pContext)
{
	setContext(pContext);
	setGuiDefDB("LOGO");

	addTask(getGuiDefDB().createInterface("LOGO"),0);

	nFrame_=0;

	// フェーダ
	pContext->getApp()->getFoward()->setFaderHandler(Scene::CFoward::FaderEvent(this,&CLogoScene::eventFade));
	pContext->getApp()->getFoward()->fadeIn(1);
	setState(START);
}

void CLogoScene::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==NORMAL)
	{
		if(++nFrame_>=45
		|| pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE
		|| pContext->getInput()->getInputState(Input::IInput::OK)==Input::IInput::RELEASE)
		{
			pContext->getInput()->guard(true);
			pContext->getApp()->getFoward()->fadeIn(20);
			setState(FADE_IN);
		}
	}
	ef(getState()==END)
	{// 終わったらタイトルへ
		getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
	}
}

void CLogoScene::OnComeBack(int nID, Task::CTaskContext*)
{
	setState(END);
}

void CLogoScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==START)
	{
		pContext->getApp()->getFoward()->fadeOut(30);
		setState(FADE_OUT);
	}
	ef(getState()==FADE_OUT)
	{
		pContext->getInput()->guard(false);
		setState(NORMAL);
	}
	ef(getState()==FADE_IN)
	{
		setState(END);
	}
}

} // namespace Logo end
} // namespace BMW end