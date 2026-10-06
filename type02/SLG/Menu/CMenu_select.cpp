/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"
#include "../CSLGScene.h"

#include "../Context/CSLGContext.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Event/CEvent.h"
#include "../Effect/CEffectMovieClip.h"

#include "../Phase/CPhase_init.h"

#include "CGameOverUnit.h"
#include "CMenu_select.h"

namespace BMW{
namespace SLG{
namespace Menu{

CMenu_select::~CMenu_select()
{
	DELETE_SAFE(pEndCtrl_);
	DELETE_SAFE(pUnit_);
}

void CMenu_select::OnReset(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	Effect::CEffectMovieClip* pEffect;

	// エンドエフェクト設定
	pEndCtrl_ = new Task::CTaskCtrl<Effect::CEffectMovieClip>;
	pEffect = p->getEffectDB().createEffect("MAP_CLEAR");
	pEndCtrl_->addTask(pEffect,Victory::VICTORY);
	pEffect = p->getEffectDB().createEffect("GAME_OVER");
	pEndCtrl_->addTask(pEffect,Victory::LOSE);

	pEndCtrl_->valid(false);
	pEndCtrl_->visible(false);

	pUnit_ = new CGameOverUnit();
	pUnit_->OnInit(p);
	pUnit_->valid(false);
	pUnit_->visible(false);
}

void CMenu_select::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	changePhase(pContext);
}

void CMenu_select::OnAction(Task::CTaskContext* pContext)
{

	switch(getState())
	{
	case END_EFFECT:
		if(pEndCtrl_->getCurrentTask()->IsEnd())
		//|| Input::releaseOK(pContext)
		//|| Input::releaseCancel(pContext))
		{// エフェクト終了ー
			pContext->getApp()->getSeDB().StopAll();
			pEndCtrl_->valid(false);
			pEndCtrl_->visible(false);
			if(pContext->top()==Victory::LOSE)
			{// 負けた時は、ダイアログ
				pContext->getInput()->cursolVisible(true);
				
				// イベントハンドラ設定
				pUnit_->setInit(GUI::CButton::ButtonEvent(this,&CMenu_select::eventButton),pContext);
				pUnit_->valid(true);
				pUnit_->visible(true);
				setState(END_DIALOG);
			}
			else
			{	setState(END);	}
		}
	break;

	case PHASE_INIT:	getTaskListCtrl()->callTaskList(Rule::PHASE_INIT,true);	break;
	//case PHASE:			getTaskListCtrl()->callTaskList(Rule::PHASE_START,true);break;

	case END_CONTINUE:
		// SLG IDを負にしておく
		pContext->setValue(-1,Flag::ID);
	case END:
		getTaskListCtrl()->returnTaskList();
		setState(NORMAL);
	break;

	case TITLE:
		// タイトルに戻るには、勝利条件フラグを潰す
		// そして、リターン
		pContext->pop();
		pContext->push(Victory::NO);
		pContext->getScene()->setState(CSLGScene::END);
		setState(NORMAL);
	break;

	default: break;
	}
}

void CMenu_select::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	setState(NORMAL);
	switch(nID)
	{
	case Rule::MENU_SELECT_PLAYER:
	case Rule::MENU_SELECT_CPU:
	case Rule::PHASE_INIT:
		// 各メニュー実行から戻ってくる時はフェーズ切り替え時か
		// 勝利条件が成立してる時
		switch(pContext->top())
		{
		case Victory::VICTORY:  // 勝利条件が成立している
		case Victory::LOSE:		// 敗北条件が成立している
			// クイックロードできない
			static_cast<CSLGContext*>(pContext)->quickLoad(false);

			pEndCtrl_->setState(pContext->top());
			pEndCtrl_->getCurrentTask()->OnReset(pContext);
			pEndCtrl_->valid(true);
			pEndCtrl_->visible(true);
			setState(END_EFFECT);
			pContext->getBgmSound()->Stop();
		break;

		default: // ↑じゃなければ
			pContext->pop();
			if(nID==Rule::MENU_SELECT_CPU
			&& pContext->getValue(Flag::PHASE)==Phase::PLAYER)
			{// 味方NPC終了
				// NPCリストを空にしてPC動作へ
				static_cast<CSLGContext*>(pContext)->getNonPlayerPhaseList().clear();
				changePhase(pContext);
			}
			else
			{// フェーズ切り替え
				if(nID!=Rule::PHASE_INIT)
				{	setState(PHASE_INIT); }
				else
				{
					setState(NORMAL);
					changePhase(pContext);
				}
			}
		break;
		}
	break;

	//case Rule::PHASE_START:
	// フェーズスタート実行後
	//	changePhase(pContext);
	//break;

	default: break;
	}	
}

void CMenu_select::callTaskAction(Task::CTaskContext* pContext)
{
	pEndCtrl_->Task(pContext);
	pUnit_->Task(pContext);
}

void CMenu_select::callTaskDraw(Task::CTaskContext* pContext)
{
	pEndCtrl_->Task(pContext);
	pUnit_->Task(pContext);
}

void CMenu_select::changePhase(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 現在のフェーズに合わせて、切り替え
	if(pContext->getValue(Flag::PHASE)==Phase::PLAYER
	&& p->getNonPlayerPhaseList().empty())
	{// 味方
		getTaskListCtrl()->callTaskList(Rule::MENU_SELECT_PLAYER,true);

		pContext->getInput()->cursolVisible(true);
	}
	else
	{// 敵もしくは、味方NPC
		getTaskListCtrl()->callTaskList(Rule::MENU_SELECT_CPU,true);
		// CPU動作中は、各種入力は無効
		pContext->getInput()->cursolVisible(false);
		if(p->getTargetMapChip()!=NULL)
			p->getTargetMapChip()->getMapChipState()->setState(Map::CMapChipState::NORMAL);
	}
	pContext->setValue(-1,Flag::TARGET_CHARA);
	pContext->setValue(-1,Flag::TARGET_MAP);
}

// イベントハンドラ
namespace{
const int mapState[3]={CMenu_select::END_CONTINUE,CMenu_select::END,CMenu_select::TITLE};
}
void CMenu_select::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 何か押された
		pContext->getInput()->guard(true);
		pContext->getInput()->cursolVisible(false);
		setState(mapState[pButton->getValue()]);
	}
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end
