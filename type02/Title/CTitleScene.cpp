#include "stdafx.h"

#include "../mode.h"
#include "../Hero/IDHero.h"

#include "../Scene/IDScene.h"
#include "../Scene/GUI/CCircleMenu.h"
#include "../Scene/GUI/CCircleMenuButton.h"

#include "IDTitle.h"
#include "CTitleScene.h"

namespace BMW{
namespace Title{

void CTitleScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト設定
	setContext(pContext);
	
	// 初期状態
	//setState(NORMAL);
	setState(FADE);

	// イベントハンドラ設定
	Scene::CFoward::FaderEvent fun;
	fun.set(this,&CTitleScene::eventFader);
	pContext->getApp()->getFoward()->setFaderHandler(fun);
	pContext->getApp()->getFoward()->fadeOut();
	nNext_=-1;

	pContext->getBgmSound()->change("TITLE");
	pContext->getBgmSound()->FadeIn(30);

	// GUI定義の読み込み
	setGuiDefDB("TITLE_GUI");

// トライアルモード時は時点無し
#ifndef BMW_TRIAL
	// 背景読み込み
	if(context_.getApp()->getGlobal().IsAmberClear())
	{// アンバークリア後はこっち
		pPanel_ = guiDef_.createInterfaceCast<GUI::CPanel>("TITLE2");
	}
	else
	{// 通常
		pPanel_ = guiDef_.createInterfaceCast<GUI::CPanel>("TITLE");
	}
#else
	// トライアルの時は通常のタイトルのみ
	pPanel_ = guiDef_.createInterfaceCast<GUI::CPanel>("TITLE");
#endif  // BMW_TRIAL

	addTask(pPanel_,INTERFACE);
	pPanel_->OnReset(&context_);


	// ボタンイベントハンドラ設定
	GUI::CPanel* pMenu = pPanel_->getWidgetCast<GUI::CPanel>("MENU");
	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CTitleScene::eventButton);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("NEWGAME"),funButton, NEW_M);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("CONTINUE"),funButton, CONTINUE_M);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("EXIT"),funButton, EXIT_M);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("LOAD"),funButton, LOAD_M);

	// トライアルモード時は時点無し
#ifndef BMW_TRIAL
	bool bClear = context_.getApp()->getGlobal().IsClear();
	GUI::CButton* pButton = pMenu->getWidgetCast<GUI::CButton>("CHARA_DIC");
	pButton->valid(bClear);
	pButton->visible(bClear);
	GUI::CButton::setButtonEvent(pButton, funButton, CHARA_M);

	pButton = pMenu->getWidgetCast<GUI::CButton>("SOUND_DIC");
	pButton->valid(bClear);
	pButton->visible(bClear);
	GUI::CButton::setButtonEvent(pButton,funButton, SOUND_M);

	if(bClear) pMenu->setY(pMenu->getDrawInfo().getY()-25);
#endif // BMW_TRIAL

	// コンテニューファイル
	setContinue(pMenu->getWidgetCast<GUI::CButton>("CONTINUE"),pContext);
}

namespace{
__inline void setContFileName(string& sFile)
{
	sFile.clear();
	sFile=BMW::sSaveFolder + "\\";
	sFile+=sContinue;
}
} // namespace end

void CTitleScene::setContinue(GUI::CButton* pButton, Task::CTaskContext* pContext)
{// コンテニューデータ設定
	string s("");
	s = sSaveFolder + "\\";
	s+=sContinue;
	// コンテニューファイルが存在しているか？
	bCont_ = CDir().IsFileExist(s);
	if(bCont_)
	{// してる！
		// セーブデータ復元
		CSerialize s;
		s.SetStoring(false);
		string sFile;
		setContFileName(sFile);
		s.Load(sFile);
		Save::CExecData& save = context_.getApp()->getExec();
		save.clear();
		s << save;
		Scenario::CDataScenario* pScn = context_.getApp()->getScenario().getScenario(save.getStory());
	#ifdef BMW_DEBUG
		// シナリオデータが無かったら飛ばす
		if(pScn==NULL) return;
	#endif
		pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pScn,false));

		// コンテニューデータから、ポップアップを生成
		string& sPopUp = pButton->getPopUp();
		sPopUp +="\n■現在のコンテニューデータ\n";
		sPopUp +="　主人公：";
		if(save.getHero()==Hero::Target::TAKUMI)
			sPopUp += "熱田 匠";
		else
			sPopUp += "望月陽菜";
		
		sPopUp +="\n　LV　　：" + CStringScanner::NumToString(save.getLv()) + "\n";
		sPopUp +="　第" + CStringScanner::NumToString(pScn->getNo()) + "話：" + pScn->getTitle();
	}
}


void CTitleScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case FADE:
		if(nNext_>=0)
		{// フェード終了後、指定されたシーンへ飛ぶ
			pContext->getApp()->getFoward()->clearPopUp();
			if(nNext_!=Scene::ID::DEMO){ getTaskListCtrl()->jumpTaskList(nNext_); }
			else{	getTaskListCtrl()->callTaskList(nNext_); }
		}
		else
		{
			pContext->getInput()->cursolVisible(true);
			pContext->getInput()->guard(false);
			setState(NORMAL);
		}
	break;

	case NEW:

		// トライアル時は存在しない
	#ifndef BMW_TRIAL
		if(context_.getApp()->getGlobal().IsClear())
		{// クリアデータがある気配
		 // 本当はセーブデータをちゃんと見るべきかもしれない 
			nNext_=Scene::ID::HANDOVER;
		}
		else
	#endif // #ifndef BMW_TRIAL
		{// クリアデータは無い
			// 主人公選択を呼ぶ
			nNext_=Scene::ID::HERO;
			// 通常モードで呼ぶ
			pContext->push(Hero::Mode::NORMAL);
		}

		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getApp()->getFoward()->fadeIn();
		pContext->getBgmSound()->FadeOut(30);
		setState(NORMAL);
	break;

	case LOAD:
		// DATAを呼ぶ
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		nNext_=Scene::ID::DATA;
		pContext->push(0); // DATAモード、タイトルから呼ばれた時は、LOADモード
		pContext->getApp()->getFoward()->fadeIn();
		pContext->getBgmSound()->FadeOut(30);
		setState(NORMAL);
	break;

	case CONTINUE:
	{
		if(bCont_)
		{// コンテニューファイルがあれば、
			pContext->push(Scene::Mode::CONTINUE);
			nNext_=Scene::ID::GAME;
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			pContext->getApp()->getFoward()->fadeIn();
			pContext->getBgmSound()->FadeOut(30);
		}
		setState(NORMAL);
	}
	break;

	// TRIAL時は無い
#ifndef BMW_TRIAL
	case CHARA:
		nNext_=Scene::ID::DICT_CHARA;
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getApp()->getFoward()->fadeIn();
		pContext->getBgmSound()->FadeOut(30);
		setState(NORMAL);
	break;

	case SOUND:
		nNext_=Scene::ID::DICT_SOUND;
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getApp()->getFoward()->fadeIn();
		pContext->getBgmSound()->FadeOut(30);
		setState(NORMAL);
	break;
#endif // BMW_TRIAL

#ifdef BMW_DEBUG
	case DEMO:
	{// デモデータ丁稚上げ
		setDemo(pContext);

		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		nNext_=Scene::ID::DEMO;
		pContext->getApp()->getFoward()->fadeIn();
		setState(NORMAL);
	}
	break;
#endif

	case EXIT: 
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->exitTaskList();
		setState(NORMAL);
	break;

	default: break;
	}
}

////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////
void CTitleScene::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{// ボタンイベントハンドラ
	// どのボタンかを判定
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// 押された
		switch(pButton->getValue())
		{
		case NEW_M:
			setState(NEW);
		break;

		case LOAD_M:
			setState(LOAD);
		break;

		case CONTINUE_M:
			setState(CONTINUE);
		break;

	/*#ifdef BMW_DEBUG
		case EXIT_M:
			setState(DEMO);
		break;
	#endif*/

	// トライアルモードの時はなかったことに
	#ifndef BMW_TRIAL
		case CHARA_M:
			setState(CHARA);
		break;

		case SOUND_M:
			setState(SOUND);
		break;
	#endif

		default: // 終了～
			pContext->getApp()->end();
		break;
		}
	}
}

void CTitleScene::eventFader(Task::CTaskContext* pContext)
{// フェード終了のイベントハンドラ
	setState(FADE);
}

} // namespace Title end
} // namespace BMW end