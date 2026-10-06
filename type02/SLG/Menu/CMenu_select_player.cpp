#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Map/CMapChipState.h"
#include "../Event/CEvent.h"
#include "../Phase/CPhaseBall.h"
#include "../Victory/CVictory_change.h"

#include "CMenu_select_OK.h"
#include "CMenu_select_Cancel.h"
#include "CMenu_select_NO.h"

#include "CMenu_select_player.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_select_player::OnReset(Task::CTaskContext* pContext)
{
	// OK監視タスク
	addTask(new CMenu_select_OK(), OK);
	// CANCEL監視タスク
	addTask(new CMenu_select_Cancel(), CANCEL);
	// NO監視タスク
	addTask(new CMenu_select_NO(), NO);
}

void CMenu_select_player::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pContext->getInput()->cursolVisible(true);
	pContext->getInput()->guard(false);
	pContext->getInput()->guardDrag(false);
	Map::CMapChipState::action(true);
}

void CMenu_select_player::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CHARA:		callRule(Rule::MENU_CHARA,pContext);	break;
	case TURN:		callRule(Rule::MENU_TURN,pContext);		break;
	case STATUS:	callRule(Rule::MENU_STATUS,pContext);	break;
	case CHARA_END: callRule(Rule::CHARA_END,pContext);		break;
	case VICTORY:	callRule(Rule::VICTORY_CHECK,pContext);		break;
	case VICTORY_CHANGE: callRule(Rule::VICTORY_CHANGE,pContext);	break;
	case END: // そしたら、リターンする
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getInput()->guardDrag(true);
		Map::CMapChipState::action(false);
	break;
	default: break;
	}
}

void CMenu_select_player::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	setState(NORMAL);
	switch(nID)
	{
	case Rule::MENU_CHARA:
	// キャラメニューから戻って来た場合は、スタックに一つ積まれている
	 // のでpopしておく
	// それが、2だったらWait_rule後なので、chara_endを呼ぶ
		if(pContext->top()==2)
		{
			pContext->getInput()->cursolVisible(false);
			setState(CHARA_END);
		}
		else
		{
			pContext->getInput()->guard(false);
			pContext->getInput()->guardDrag(false);
			Map::CMapChipState::action(true);
		}
		pContext->pop();
	break;

	case Rule::MENU_STATUS:
		pContext->getInput()->guard(false);
		pContext->getInput()->guardDrag(false);
		Map::CMapChipState::action(true);
	break;
	
	case Rule::MENU_TURN:
	// ターンメニューから戻って来た場合は、フェイズが切り替わってるんじゃない？
		if(pContext->getValue(Flag::PHASE_CHANGE)==1)
		{// 切り替わってる
			// フラグは倒す
			pContext->setValue(0,Flag::PHASE_CHANGE);
			pContext->push(Victory::NO);
			setState(END);
		}
		else
		{// 別にそういうわけじゃない
			pContext->getInput()->guard(false);
			pContext->getInput()->guardDrag(false);
			Map::CMapChipState::action(true);
		}
	break;

	case Rule::CHARA_END:
		// キャラエンド後には、勝利条件成立フラグ立ってるかもしれない
		// ので、チェック
		setState(VICTORY);
	break;

	case Rule::VICTORY_CHECK:
		switch(pContext->top())
		{
		case Victory::VICTORY:	// 勝利条件が成立
		case Victory::LOSE:		// 敗北条件が成立
			// ↑のいずれかが成立しているようだったら、リターンする
			setState(END);
		break;

		case Victory::EXPERT:
		// 熟練度条件が成立
			pContext->pop();
			// 熟練度を+1にして、熟練度判定を無効にする
			pContext->setValue(pContext->getValue(Flag::EXPERT)+1,Flag::EXPERT);
			// 熟練度獲得をお知らせ
			pContext->push(Victory::CVictory_change::EXPERT_GET);
			setState(VICTORY_CHANGE);
			static_cast<CSLGContext*>(pContext)->getEvent()->getTurnBall().update(pContext);
		break;

		default:
		// 特に成立していない
			pContext->getInput()->cursolVisible(true);
			pContext->getInput()->guard(false);
			pContext->getInput()->guardDrag(false);
			Map::CMapChipState::action(true);
			pContext->pop();
			pContext->setValue(-1,Flag::TARGET_CHARA);
			pContext->setValue(-1,Flag::TARGET_MAP);
		break;
		}
	break;

	case Rule::VICTORY_CHANGE:
		// 勝利条件が同時に設定されてるかもしれないので、
		// もう一回、チェック
		setState(VICTORY);
	break;

	default: break;
	}
}

void CMenu_select_player::callRule(int nID,Task::CTaskContext* pContext)
{
	getTaskListCtrl()->callTaskList(nID,true);
	pContext->getInput()->guard(true);
	pContext->getInput()->guardDrag(true);
	Map::CMapChipState::action(false);
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end