#include "stdafx.h"

#include "../Scene/IDScene.h"

#include "IDADV.h"

#include "CADVBack.h"
#include "CMsgBoard.h"
#include "CAdvFactory.h"
#include "CAdvVM.h"

#include "CADVScene.h"

namespace BMW{
namespace ADV{

void CADVScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキストのスタックトップの値が、
	// 難易度・生成するADV IDと積まれている
	context_.setValue(pContext->top(),Flag::ID);
	pContext->pop();

	// コンテキスト設定
	setContext(pContext);

	// GUI定義読み込み
	setGuiDefDB("ADV");

	// 背景
	CADVBack* pBack = new CADVBack();
	pBack->OnInit(&context_);
	addTask(pBack,BACK);
	context_.setBack(smart_ptr<CADVBack>(pBack,false));

	// メッセージ LEFT
	CMsgBoard* pBoard = new CMsgBoard();
	pBoard->setSide(CMsgBoard::LEFT);
	pBoard->OnInit(&context_);
	addTask(pBoard,MSG_L);
	context_.setMsgBoard(smart_ptr<CMsgBoard>(pBoard,false),CMsgBoard::LEFT);

	// メッセージ RIGHT
	pBoard = new CMsgBoard();
	pBoard->setSide(CMsgBoard::RIGHT);
	pBoard->OnInit(&context_);
	addTask(pBoard,MSG_R);
	context_.setMsgBoard(smart_ptr<CMsgBoard>(pBoard,false),CMsgBoard::RIGHT);

	// ADV VM
	CAdvVM* pVM = new CAdvVM();
	addTask(pVM,VM);
	CAdvFactory* pFactory = new CAdvFactory();
	pFactory->OnInit(context_);
	pFactory->setScript(pContext->getScenarioData()->getScenarioFile(context_.getValue(Flag::ID)),
						context_);
	pVM->setTaskListFactory(smart_ptr<Task::ITaskListFactory>(pFactory));
	pVM->jumpTaskList(Rule::MAIN);

	pContext->getInput()->cursolVisible(false);
	pContext->getInput()->guard(true);

	pContext->getApp()->skip(false);

#ifdef BMW_DEBUG
	pInput_ = static_cast<Input::CTaskInput*>(pContext->getInput());
#endif
}

void CADVScene::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==END)
	{// 終了
	#ifdef BMW_DEBUG
		// デバッグ時には、自分の-1をスタックに積む
		pContext->push(-1);
	#endif
		getTaskListCtrl()->returnTaskList();
		pContext->getApp()->animeSkip();
	}
	#ifdef BMW_DEBUG
	else if(getState()==CONTINUE)
	{
		getTaskListCtrl()->returnTaskList();
		pContext->getApp()->animeSkip();
	}
	else if(pInput_->getKeyBoard().IsKeyPush(DIK_BACK))
	{// デバッグモード時のみBSを押すとADVを最初からやり直せる
		setState(NORMAL);
		Scene::CFoward* pForward = pContext->getApp()->getFoward();
		pForward->setFadeColor(RGB(0,0,0));
		Scene::CFoward::FaderEvent fun;
		fun.set(this,&CADVScene::eventFade);
		pForward->setFaderHandler(fun);
		pForward->fadeIn();
		pContext->push(context_.getValue(Flag::ID));
		pContext->getApp()->getBgm()->Stop();
	}
	else if(pInput_->getKeyBoard().IsKeyPush(DIK_RETURN))
	{// デバッグモード時のみENTERを押すとタイトルに飛ぶ
		setState(NORMAL);
		Scene::CFoward* pForward = pContext->getApp()->getFoward();
		pForward->setFadeColor(RGB(0,0,0));
		Scene::CFoward::FaderEvent fun;
		fun.set(this,&CADVScene::eventFade);
		pForward->setFaderHandler(fun);
		pForward->fadeIn();
		pContext->push(-2);
	}
	#endif
}

#ifdef BMW_DEBUG
void CADVScene::eventFade(Task::CTaskContext* pContext)
{
	setState(CONTINUE);
}
#endif

void CADVScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	pContext->getInput()->cursolVisible(false);
	pContext->getInput()->guard(true);
}

} // namespace ADV end
} // namespace BMW end