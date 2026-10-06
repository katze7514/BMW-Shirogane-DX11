#include "stdafx.h"

#include "../Scene/Unit/CYesNoUnit.h"
#include "../Scene/IDScene.h"

#include "../Data/IDData.h"
#include "../Hero/IDHero.h"

#include "../ADV/Code/CCode_train.h"

#include "CHandoverScene.h"

namespace BMW{
namespace Handover{

CHandoverScene::CHandoverScene()
{
	pYesNo_ = new Unit::CYesNoUnit();
}

CHandoverScene::~CHandoverScene()
{
	DELETE_SAFE(pYesNo_);
	DELETE_SAFE(pKakunin_);
	DELETE_SAFE(pEnemyTrain_);
}

void CHandoverScene::Task(Task::CTaskContext* pContext)
{
	context_.action(pContext->IsAction());

	pCurrentPanel_->Task(&context_);

	// YES_NOが描画的に上に来る
	pYesNo_->Task(&context_);

	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

void CHandoverScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト設定
	setContext(pContext);

	// GUI読み込み
	setGuiDefDB("HANDOVER");

	// YES_NO設定
	pYesNo_->OnInit(&context_);
	pYesNo_->setX(320);
	pYesNo_->setY(400);
	GUI::CButton::ButtonEvent fun(this, &CHandoverScene::eventButton);
	pYesNo_->setButtonHandler(fun, YES, NO);
	Unit::CYesNoUnit::CancelEvent funCancel(this, &CHandoverScene::eventCancel);
	pYesNo_->setCancelHandler(funCancel);

	// 確認パネル取得
	pKakunin_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_HIKITSUGISENTAKU");

	// 養成段階取得
	pEnemyTrain_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_YOUSEISENTAKU");
	// 黒幕取得
	pBlack_ = pEnemyTrain_->getWidgetCast<GUI::CGraphic>("BLACK");
	pBlack_->visible(false);
	// ボタンハンドラ設定
	pNumButton_ = pEnemyTrain_->getWidgetRecCast<GUI::CPanel>("PANEL_YOUSEIDANKAI/PANEL_YOUSEINUM");
	for(int i=0; i<=10; ++i)
		GUI::CButton::setButtonEvent(pNumButton_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("YOUSEI_BUTTON_",i)),fun,TRAIN+i);

	// 最初は確認から
	nNextScene_=KAKUNIN;
	setNextPanel(nNextScene_);

	// ひとまず、フェード
	Scene::CFoward::FaderEvent funFade(this,&CHandoverScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(funFade);
	// FadeOut
	context_.getApp()->getFoward()->fadeOut();
	setState(FADE);
}

void CHandoverScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case KAKUNIN: // 確認
		// キャンセルしたら、タイトルへ
		if(Input::releaseCancel(pContext))
		{
			nNextScene_=Scene::ID::TITLE;
			context_.getInput()->cursolVisible(false);
			context_.getInput()->guard(true);
			context_.getApp()->getFoward()->fadeIn();

			setState(FADE_END);
		}
	break;

	case ENEMY_TRAIN: // 養成段階選択中
		if(!pBlack_->IsVisible() // ブラック出てる時はYES_NOが出てるので、そっちで処理
		&& Input::releaseCancel(pContext))
		{// 確認へ戻る
			nNextScene_=KAKUNIN;
			context_.getInput()->cursolVisible(false);
			context_.getInput()->guard(true);
			context_.getApp()->getFoward()->fadeIn();
			setState(FADE_IN);
		}
	break;

	case PANEL_START:
		setNextPanel(nNextScene_);
		context_.getApp()->getFoward()->fadeOut();
		setState(FADE);
	break;

	default: break;
	}
}

void CHandoverScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	Scene::CFoward::FaderEvent funFade(this,&CHandoverScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(funFade);

	// DATAから戻って来た！
	if(pContext->top()>=0)
	{//  ロードされてる！　じゃ、養成段階選択
		nNextScene_=ENEMY_TRAIN;
	}
	else
	{// キャンセルされたのかYO
	 // 確認へ戻る
		nNextScene_=KAKUNIN;
	}
	pContext->pop();
	setState(PANEL_START);
}

/////////////////////////////////
// イベントハンドラ
/////////////////////////////////
void CHandoverScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{// FADE_OUT
		context_.getInput()->cursolVisible(true);
		context_.getInput()->guard(false);
		setState(nNextScene_);
	}
	ef(getState()==FADE_IN)
	{// ゲーム内ジャンプ
		setNextPanel(nNextScene_);
		context_.getApp()->getFoward()->fadeOut();
		setState(FADE);
	}
	else
	{// 次のシーンへうつるジャンプ
		pContext->getApp()->getFoward()->clearPopUp();
		if(nNextScene_==Scene::ID::DATA)
		{	
			pContext->push(Data::Mode::HANDOVER);
			getTaskListCtrl()->callTaskList(nNextScene_,true);
		}
		else // データ以外へはジャンプ！
		{
			// 主人公選択へ行くなら、モードを積んでおく
			if(nNextScene_==Scene::ID::HERO)
				pContext->push(nHero_);
			// ジャンプ！
			getTaskListCtrl()->jumpTaskList(nNextScene_);
		}
		setState(NORMAL);
	}
}

void CHandoverScene::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{
		switch(pButton->getValue())
		{
		case YES:
			if(getState()==KAKUNIN)
			{// 確認状態ならデータへ
				nNextScene_=Scene::ID::DATA;
			}
			ef(getState()==ENEMY_TRAIN)
			{// 敵養成段階だったら、主人公へ
				saveHandover(); // 引き継ぎ処理
				nNextScene_=Scene::ID::HERO;
				nHero_=Hero::Mode::HANDOVER;
			}
			context_.getInput()->cursolVisible(false);
			context_.getInput()->guard(true);
			context_.getApp()->getFoward()->fadeIn();
			setState(FADE_END);
		break;

		case NO:
			if(getState()==KAKUNIN)
			{// 確認状態なら通常状態でHEROへ
				nNextScene_=Scene::ID::HERO;
				nHero_=Hero::Mode::NORMAL;
				context_.getInput()->cursolVisible(false);
				context_.getInput()->guard(true);
				context_.getApp()->getFoward()->fadeIn();

				setState(FADE_END);
			}
			ef(getState()==ENEMY_TRAIN)
			{// 敵養成なら、閉じるだけ
				// 選択解除
				trainButtonPause(false);
				// YES_NOを閉じるだけ
				pBlack_->visible(false);
				pYesNo_->valid(false);
				pYesNo_->visible(false);
			}
			
		break;

		default:
			if(getState()==ENEMY_TRAIN)
			{// 養成段階選びだったら、養成段階そのもの！
				nEnemyTrain_ = pButton->getValue();
				// ボタンを一時停止
				trainButtonPause(true);
				// YES_NO表示
				pBlack_->visible(true);
				pYesNo_->setIntro(-1,pContext,-1);
				pYesNo_->valid(true);
				pYesNo_->visible(true);
			}
		break;
		}
	}
}

void CHandoverScene::eventCancel(Task::CTaskContext* pContext)
{
	if(getState()==ENEMY_TRAIN)
	{// 敵養成段階の時だけ反応
		pContext->getInput()->resetInputState();
		// 選択解除
		trainButtonPause(false);
		// YES_NOを閉じるだけ
		pBlack_->visible(false);
		pYesNo_->valid(false);
		pYesNo_->visible(false);
	}
}


////////////////////////////////
// ヘルパ
////////////////////////////////
void CHandoverScene::setNextPanel(int nNext)
{
	if(nNext==KAKUNIN)
	{
		// 入れ替え
		pCurrentPanel_=pKakunin_;
		pYesNo_->setAsk(-1);
		pYesNo_->setState(Unit::CYesNoUnit::NORMAL);
		pYesNo_->setAlpha(255);
		pYesNo_->valid(true);
		pYesNo_->visible(true);
	}
	else
	{
		// 入れ替え
		pCurrentPanel_ = pEnemyTrain_;
		pBlack_->visible(false);
		pYesNo_->valid(false);
		pYesNo_->visible(false);
	}
}

////////////////////////////////
// 引継ぎ処理
////////////////////////////////
void CHandoverScene::saveHandover()
{
	using ADV::Code::CCode_train;
	// DATAシーンによってクリアデータがロードされてるはず
	Save::CExecData& exec = context_.getApp()->getExec();
	// クリア回数増
	exec.incClear();
	// BP・FP還元
	int nBP = exec.getBP();
	int nFP = exec.getFP();
	 // 有効なキャラの養成BP・FPをバックする
	set<int>::iterator it = exec.beginValid();
	while(!exec.endValid())
	{
		exec.setBP(0);
		exec.setFP(0);
		CCode_train::ctrlTrain(*exec.nextValid(),CCode_train::BACK,-1,-1,-1,&context_);
		nBP += exec.getBP();
		nFP += exec.getFP();
	}

	// BP・FPを半分に
	exec.setBP(nBP/2);
	exec.setFP(nFP/2);

	// 敵養成段階設定
	exec.setFlag("ENEMY_TRAIN",nEnemyTrain_);

	// フラグ設定
	exec.setFlag("VICTORY",0);

	// アンバークリア済みなら、リリィとアンバーが出てくる
	if(context_.getApp()->getGlobal().IsAmberClear())
		exec.setFlag("HANDOVER",2);
	else
		exec.setFlag("HANDOVER",1);
}

//////////////////////////////
// ボタン停止/解除
//////////////////////////////
void CHandoverScene::trainButtonPause(bool bPause)
{
	GUI::CButton* pButton = pNumButton_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("YOUSEI_BUTTON_",nEnemyTrain_));
	pButton->setState(bPause ? GUI::IButton::PRESS : GUI::IButton::NORMAL);
	pButton->valid(!bPause);
}

} // namespace Handover end
} // namesapce BMW end
