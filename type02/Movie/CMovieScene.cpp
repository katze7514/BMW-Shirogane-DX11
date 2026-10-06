#include "stdafx.h"

#include "DB/CSymbolDB.h"
#include "CMovieScene.h"

namespace BMW{
namespace Movie{

CMovieScene::CMovieScene()
{
	symbol_ = new Movie::CSymbolDB();
}

CMovieScene::~CMovieScene()
{
	DELETE_SAFE(pMovie_);
	DELETE_SAFE(symbol_);
}

void CMovieScene::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction()) OnAction(pContext);
	pMovie_->Task(pContext);
}

void CMovieScene::OnInit(Task::CTaskContext* pContext)
{
	pContext->getInput()->cursolVisible(false);
	
	// Movieシンボル読み込み
	symbol_->setSymbol(Config::Const::configDB_.getConfigFile(pContext->top()));
	pContext->pop();
	// Movieムービー取得
	pMovie_ = static_cast<Movie::CMovieClip*>(symbol_->createSymbolStr("MAIN"));
	pMovie_->OnReset(pContext);
	// とりあえず、ストップ
	pMovie_->valid(false);

	Scene::CFoward::FaderEvent funFade;
	funFade.set(this,&CMovieScene::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(funFade);
	pContext->getApp()->getFoward()->fadeOut(30);
	
	setState(FADE);
}

void CMovieScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case PLAY:
		if(pContext->getInput()->getInputState(Input::IInput::OK)==Input::IInput::RELEASE
		|| pMovie_->IsEnd())
		{
			pContext->getInput()->guard(true);
			pContext->getApp()->getFoward()->fadeIn(15);
			setState(WAIT);
		}
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

/////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////
void CMovieScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		pMovie_->valid(true);
		pContext->getInput()->guard(false);
		setState(PLAY); 
	}
	ef(getState()==WAIT)
	{ 
		setState(END);
	}
}


} // namespace Movie end
} // namespace BMW end