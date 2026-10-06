#include "stdafx.h"

#include "../mode.h"
#include "../Scene/IDScene.h"
#include "../Scene/Unit/CYesNoUnit.h"

#include "CEdScene.h"

namespace BMW{
namespace ED{

void CEdScene::OnInit(Task::CTaskContext* pContext)
{
	// RETURNかどうかフラグ取得
	nReturn_ = pContext->top();
	pContext->pop();

	nFrame_=0;

#ifdef BMW_DEBUG
	CDbg().Out("ED Scene %d",nReturn_);
#endif

	pContext->getInput()->cursolVisible(false);
	setContext(pContext);
	// インターフェイス読み込み
	setGuiDefDB("ED");
	// 背景
	addTask(getGuiDefDB().createInterface("BACK_ED"),BACK);

	// EDシンボル読み込み
	getGuiDefDB().getSymbolDB().setSymbol(pContext->getScenarioData()->getScenarioFile(pContext->top()));
	pContext->pop();
	// EDムービー取得
	pED_ = static_cast<Movie::CMovieClip*>(getGuiDefDB().getSymbolDB().createSymbolStr("ED"));
	pED_->OnReset(pContext);
	// とりあえず、ストップ
	pED_->valid(false);
	addTask(pED_,MOVIE);

	// YES/NOユニット
	pYesNo_ = new Unit::CYesNoUnit();
	pYesNo_->OnInit(&context_);
	pYesNo_->valid(false);
	pYesNo_->visible(false);
	addTask(pYesNo_,YESNO);

	pYesNo_->setX(320);
	pYesNo_->setY(240);

	// イベントハンドラ設定
	pYesNo_->setButtonHandler(GUI::CButton::ButtonEvent(this,&CEdScene::eventYesNo),YES,NO);
	pYesNo_->setCancelHandler(Unit::CYesNoUnit::CancelEvent(this,&CEdScene::eventCancel));


	// クリアしてたら
	Save::CExecData& exec = pContext->getApp()->getExec();
	// クリアストーリーをクリアとしてしまう
	if(exec.getNextScenario()==pContext->getApp()->getScenario().getScenarioID("CLEAR_NORMAL")
	|| exec.getNextScenario()==pContext->getApp()->getScenario().getScenarioID("CLEAR_GOOD")
	|| exec.getNextScenario()==pContext->getApp()->getScenario().getScenarioID("CLEAR_TRUE"))
		exec.setStory(exec.getNextScenario());

	// フェード
	Scene::CFoward::FaderEvent funFade;
	funFade.set(this,&CEdScene::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(funFade);
	pContext->getApp()->getFoward()->fadeOut(30);
	
	setState(FADE);
}

void CEdScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case PLAY:
		if(pED_->IsEnd())
		{
			setState(PLAY_WAIT);
			pContext->getInput()->guard(false);
		}
	break;

	case PLAY_WAIT:
		if(++nFrame_>=120
		|| Input::releaseCancel(&context_)
		|| Input::releaseOK(&context_))
		{
			pContext->getApp()->getFoward()->fadeIn(15);
			setState(WAIT);
			pContext->getInput()->guard(true);
		}
	break;

	case DATA:
		// こっちだと、SAVEモード
		pContext->push(2);
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->callTaskList(BMW::Scene::ID::DATA,true);
		setState(DIALOG);
	break;

	case END:
	#ifdef BMW_DEBUG
		CDbg().Out("ED END %d", nReturn_);
	#endif
		if(0==nReturn_)
			getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		else // RETURNモードだったらリターンする
			getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CEdScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{// データシーンから戻ってきたら、終了
	setState(END);
}

/////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////
void CEdScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		setState(PLAY); 
		pED_->valid(true);
	}
	ef(getState()==WAIT)
	{ 
		pED_->valid(false);
		pED_->visible(false);
		setState(DIALOG_WAIT);
		pContext->getApp()->getFoward()->fadeOut(15);
	}
	ef(getState()==DIALOG_WAIT)
	{
		pYesNo_->valid(true);
		pYesNo_->visible(true);
		pContext->getInput()->guard(false);
		pContext->getInput()->cursolVisible(true);
		pYesNo_->setIntro(Unit::CYesNoUnit::SAVE,pContext);
		setState(DIALOG);
	}
	ef(getState()==DATA_WAIT)
	{
		setState(DATA);
	}
	ef(getState()==END_WAIT)
	{
		setState(END);
	}
}

void CEdScene::eventYesNo(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// なにか押された
		if(pButton->getValue()==YES)
		{// YESボタンや！
		 // データシーンを呼び出すで！！
			pContext->getInput()->guard(true);
			pContext->getInput()->cursolVisible(false);
			pContext->getApp()->getFoward()->fadeIn(15);
			pYesNo_->valid(false);
			setState(DATA_WAIT);
		}
		else
		{// NOボタンや！
			eventCancel(pContext);
		}
	}
}

void CEdScene::eventCancel(Task::CTaskContext* pContext)
{
	// キャンセルされたら、終了
	pContext->getApp()->getFoward()->fadeIn(15);
	pContext->getInput()->guard(true);
	pContext->getInput()->cursolVisible(false);
	pYesNo_->valid(false);
	setState(END_WAIT);
}

} // namespace ED end
} // namespace BMW end