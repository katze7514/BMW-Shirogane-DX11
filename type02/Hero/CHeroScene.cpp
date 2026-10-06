#include "stdafx.h"

#include "../Scene/IDScene.h"
#include "../Scene/Unit/IDHelp.h"

#include "IDHero.h"
#include "CHeroScene.h"

namespace BMW{
namespace Hero{

void CHeroScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト設定
	setContext(pContext);

	// モード取得
	context_.setValue(pContext->top(), Flag::MODE);
	pContext->pop();

	// GUI生成
	setGuiDefDB("HERO");
	// 音
	context_.getBgmSound()->change("STATUS");
	context_.getBgmSound()->FadeIn(30);

	// インターフェイス
	pPanel_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("HEROSELECT");
	addTask(pPanel_,0);
	// インターフェイス展開
	pStand_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("STAND");
	pTakumi_ = pPanel_->getWidgetCast<GUI::CButton>("SUPER");
	pHaruna_ = pPanel_->getWidgetCast<GUI::CButton>("REAL");
	pProfile_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("PROFILE");
	
	// イベントハンドラ
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CHeroScene::eventButton);
	GUI::CButton::setButtonEvent(pTakumi_,fun,TAKUMI);
	GUI::CButton::setButtonEvent(pHaruna_,fun,HARUNA);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("OK"),fun,OK);
	
	// フェードアウト
	Scene::CFoward::FaderEvent funFade;
	funFade.set(this,&CHeroScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(funFade);
	context_.getApp()->getFoward()->fadeOut();
	setState(INTRO);

	// はじめは陽菜を選択
	context_.setValue(Target::HARUNA,Flag::TARGET_HERO);
	pHaruna_->setState(GUI::CButton::OVER);
	pHaruna_->valid(false);
	pStand_->validWidget("HARUNA");
	pProfile_->validWidget("HARUNA");

	motion_[0].setStart(pStand_->getDrawInfo());
	motion_[0].setStep(4);
	motion_[0].setEdging(-100);
	motion_[1].setEnd(pStand_->getDrawInfo());
	motion_[1].setStep(6);
	motion_[1].setEdging(100);
}

void CHeroScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{
			// 何もせずにタイトルへ戻る
			context_.getInput()->cursolVisible(false);
			context_.getInput()->guard(true);
			context_.getBgmSound()->FadeOut(30);
			context_.setValue(Scene::ID::TITLE,Flag::NEXT);
			context_.getApp()->getFoward()->fadeIn();
			setState(END);
		}
	break;

	case GAME:
		pContext->push(Scene::Mode::NEW);
		setState(END);
	break;

	case CHANGE_OUT:
		motion_[0].inc();
		pStand_->setX(motion_[0].getX());
		if(motion_[0].IsEnd())
		{
			pStand_->validWidget(sID_);
			pProfile_->validWidget(sID_);
			setState(CHANGE_IN);
		}
	break;

	case CHANGE_IN:
		motion_[1].inc();
		pStand_->setX(motion_[1].getX());
		if(motion_[1].IsEnd())
		{
			context_.getInput()->guard(false);
			setState(NORMAL);
		}
	break;
	}
}
///////////////////////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////////////////////
void CHeroScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==INTRO)
	{
		context_.getInput()->cursolVisible(true);
		context_.getInput()->guard(false);
		setState(NORMAL);
	}
	else
	{			
		getTaskListCtrl()->jumpTaskList(context_.getValue(Flag::NEXT));
	}
}

void CHeroScene::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		context_.getInput()->guard(true);
		switch(pButton->getValue())
		{
		case TAKUMI:
			context_.setValue(Target::TAKUMI,Flag::TARGET_HERO);
			pTakumi_->setState(GUI::CButton::OVER);
			pTakumi_->valid(false);
			pHaruna_->valid(true);
			sID_="TAKUMI";
			setState(CHANGE_OUT);

			motion_[0].setEnd(-180);
			motion_[0].reset();

			motion_[1].setStart(-200);
			motion_[1].reset();
		break;

		case HARUNA:
			context_.setValue(Target::HARUNA,Flag::TARGET_HERO);
			pHaruna_->setState(GUI::CButton::OVER);
			pHaruna_->valid(false);
			pTakumi_->valid(true);
			sID_="HARUNA";
			setState(CHANGE_OUT);

			motion_[0].setEnd(-200);
			motion_[0].reset();

			motion_[1].setStart(-180);
			motion_[1].reset();
		break;

		case OK:
			// 新規セーブデータを生成し、GameSceneへ
			actionSave(pContext);
			context_.getInput()->cursolVisible(false);
			context_.getBgmSound()->FadeOut(30);
			context_.setValue(Scene::ID::GAME,Flag::NEXT);
			context_.getApp()->getFoward()->fadeIn();
			setState(GAME);
		break;

		default: break;
		}
	}
}

///////////////////////////////////////////////////////////
// アクション
///////////////////////////////////////////////////////////
void CHeroScene::actionSave(Task::CTaskContext* pContext)
{
	int nStartScenario = context_.getApp()->getScenario().getScenarioID("01_C");
	// 新規セーブデータを生成
	Save::CExecData& save = context_.getApp()->getExec();
	// 現在のデータをクリア
	if(context_.getValue(Flag::MODE)==Mode::NORMAL)
	{// 通常のNEW GAME
		save.clear();
		// ヘルプフラグをすべてOFFにする
		Unit::Help::resetHelpFlag(save);
		// 第一話から
		save.setStory(nStartScenario);
	}
	else 
	{// 引き継ぎ後の選択
		save.clearHandover();
		// 引き継ぎ用シナリオから
		save.setStory(context_.getApp()->getScenario().getScenarioID("HANDOVER"));
	}

	// 主人公を設定
	save.setHero(context_.getValue(Flag::TARGET_HERO));
	// 最初のシナリオを設定する
	save.setNextScenario(nStartScenario);
	// 初期データを設定
	// 有効なキャラリスト
	save.addValid(Chara::Const::charaID_.getValue("PLAYER_HARUNA"));
	save.addValid(Chara::Const::charaID_.getValue("PLAYER_TAKUMI"));
	// 初期養成データ
	save.getTrainData(Chara::Const::charaID_.getValue("PLAYER_HARUNA"));
	save.getTrainData(Chara::Const::charaID_.getValue("PLAYER_TAKUMI"));
	// エース
	save.setAce(Chara::Const::charaID_.getValue(context_.getValue(Flag::TARGET_HERO)==Hero::Target::TAKUMI
												? "PLAYER_TAKUMI" : "PLAYER_HARUNA"));
}

} // namespace Hero end
} // namespace BMW end