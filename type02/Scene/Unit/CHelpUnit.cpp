#include "stdafx.h"

#include "../../GUI/DB/CGuiDefDB.h"

#include "IDHelp.h"
#include "CHelpUnit.h"

namespace BMW{
namespace Unit{

CHelpUnit::CHelpUnit()
{
	pGuiDef_ = new GUI::CGuiDefDB();
}

CHelpUnit::~CHelpUnit()
{
	DELETE_SAFE(pPanel_[0]);
	DELETE_SAFE(pPanel_[1]);
	DELETE_SAFE(pGuiDef_);
}

void CHelpUnit::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			OnAction(pContext);
			pPanel_[TOP]->Task(pContext);
			pPanel_[BOTTOM]->Task(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			pPanel_[TOP]->Task(pContext);
			pPanel_[BOTTOM]->Task(pContext);
		}
	}
}

void CHelpUnit::OnInit(Task::CTaskContext* pContext)
{
	// インターフェイス設定
	pGuiDef_->setGuiDef(Config::Const::configDB_.getConfigFileStr("HELP"));
	// 上
	pPanel_[TOP] = pGuiDef_->createInterfaceCast<GUI::CPanel>("PANEL_HELP_TOP");
	motion_[TOP].setStart(0,-101);
	motion_[TOP].setEnd(0,0);
	// 下
	pPanel_[BOTTOM] = pGuiDef_->createInterfaceCast<GUI::CPanel>("PANEL_HELP_BOTTOM");
	motion_[BOTTOM].setStart(0,634);
	motion_[BOTTOM].setEnd(0,480);

	// スタックトップに出すヘルプIDが入ってる
	int nHelp = pContext->top();
	pContext->pop();

	const string sTextID[2][Help::HELP_END]=
	{// ヘルプIDとインターフェイスIDのマップ
		{// タイトル（上に表示）
			"SLG_START_TITLE",
			"SLG_START2_TITLE",
			"SLG_CHARA_SELECT_TITLE",
			"SLG_CHARA_SELECT2_TITLE",
			"SLG_CHARA_METOR_TITLE",
			"SLG_CHARA_MENU_TITLE",
			"SLG_MOVE_SELECT_TITLE",
			"SLG_ATK_TITLE",
			"SLG_ATK_SELECT_TITLE",
			"SLG_ATK_CHECK_TITLE",
			"SLG_ATK_STATUS_TITLE",
			"SLG_ATK_COND_TITLE",
			"SLG_ATK_COLLAB_TITLE",
			"SLG_ATK_FIELD_CENTER_TITLE",
			"SLG_ATK_FIELD_LINE_TITLE",
			"SLG_ATK_FIELD_THROW_TITLE",
			"SLG_SPIRIT_TITLE",
			//"SLG_ITEM_TITLE",
			"SLG_CURE_TITLE",
			"SLG_PIT_TITLE",
			"SLG_TURN_MENU_TITLE",
			"SLG_ICHIRAN_TITLE",
			"SLG_SAVE_TITLE",
			"INTER_SELECT_TITLE",
			"INTER_FUND_TITLE",
			"INTER_SKILL_TITLE",
			"INTER_BATTLE_TITLE",
			"INTER_WEAPON_TITLE",
			"INTER_ITEM_TITLE",
			"INTER_ITEM_CHANGE_TITLE"
		}
		,
		{// 内容（下に表示）
			"SLG_START_TEXT",
			"SLG_START2_TEXT",
			"SLG_CHARA_SELECT_TEXT",
			"SLG_CHARA_SELECT2_TEXT",
			"SLG_CHARA_METOR_TEXT",
			"SLG_CHARA_MENU_TEXT",
			"SLG_MOVE_SELECT_TEXT",
			"SLG_ATK_TEXT",
			"SLG_ATK_SELECT_TEXT",
			"SLG_ATK_CHECK_TEXT",
			"SLG_ATK_STATUS_TEXT",
			"SLG_ATK_COND_TEXT",
			"SLG_ATK_COLLAB_TEXT",
			"SLG_ATK_FIELD_CENTER_TEXT",
			"SLG_ATK_FIELD_LINE_TEXT",
			"SLG_ATK_FIELD_THROW_TEXT",
			"SLG_SPIRIT_TEXT",
			//"SLG_ITEM_TEXT",
			"SLG_CURE_TEXT",
			"SLG_PIT_TEXT",
			"SLG_TURN_MENU_TEXT",
			"SLG_ICHIRAN_TEXT",
			"SLG_SAVE_TEXT",
			"INTER_SELECT_TEXT",
			"INTER_FUND_TEXT",
			"INTER_SKILL_TEXT",
			"INTER_BATTLE_TEXT",
			"INTER_WEAPON_TEXT",
			"INTER_ITEM_TEXT",
			"INTER_ITEM_CHANGE_TEXT"
		}
	};
	// IDに合わせてテキストをセット
	Task::ITaskBase* pTitle = pGuiDef_->createInterface(sTextID[TOP][nHelp]);
	pPanel_[TOP]->swapWidget(pTitle,"TITLE");
	// ↑で上書きされてしまうのでここで設定
	pTitle->setHeight(128);

	pPanel_[BOTTOM]->swapWidget(pGuiDef_->createInterface(sTextID[BOTTOM][nHelp]),"TEXT");

	// 登場動作設定
	// 現在の入力フラグを保存
	bInputGuard_	= pContext->getInput()->IsGuard();
	bCursolVisible_ = pContext->getInput()->IsCursolVisible();
	// シーンの動作を止める
	pContext->getApp()->stopScene(true);
	pContext->getInput()->guard(true);
	pContext->getInput()->cursolVisible(false);
	// モーション設定
	setMotion(true);
	// 登場から
	setState(INTRO);

	// 何か音ならそう！
}

void CHelpUnit::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INTRO:
		motion_[TOP].inc();
		pPanel_[TOP]->setDrawInfo(motion_[TOP]);
		motion_[BOTTOM].inc();
		pPanel_[BOTTOM]->setDrawInfo(motion_[BOTTOM]);
		if(motion_[TOP].IsEnd() && motion_[BOTTOM].IsEnd())
		{// 動作終了
			setState(WAIT);
			pContext->getInput()->guard(false);
		}
	break;

	case WAIT:
		if(Input::releaseOK(pContext) || Input::releaseCancel(pContext))
		{// 何かボタン押されたら、終了
			pContext->getInput()->guard(true);
			setMotion(false);
			setState(END);
		}
	break;

	case END:
		motion_[TOP].dec();
		pPanel_[TOP]->setDrawInfo(motion_[TOP]);
		motion_[BOTTOM].dec();
		pPanel_[BOTTOM]->setDrawInfo(motion_[BOTTOM]);
		if(motion_[TOP].IsStart() && motion_[BOTTOM].IsStart())
		{// 動作終了
			setState(NORMAL);
			pContext->getApp()->stopScene(false);
			// 入力フラグはヘルプ動作前の状態に戻る
			pContext->getInput()->guard(bInputGuard_);
			pContext->getInput()->cursolVisible(bCursolVisible_);
			// このタスクはいらなくなる
			pContext->getTaskList()->killMe();
		}
	break;
	
	default: break;
	}
}

void CHelpUnit::setMotion(bool bIntro)
{
	motion_[TOP].setStep(bIntro ? 15 : 10);
	motion_[TOP].setEdging(bIntro ? 80 : -80);
	motion_[TOP].reset(!bIntro);
	pPanel_[TOP]->setDrawInfo(motion_[TOP]);

	motion_[BOTTOM].setStep(bIntro ? 15 : 10);
	motion_[BOTTOM].setEdging(bIntro ? 80 : -80);
	motion_[BOTTOM].reset(!bIntro);
	pPanel_[BOTTOM]->setDrawInfo(motion_[BOTTOM]);
}

} // namespace Unit end
} // namespace BMW end