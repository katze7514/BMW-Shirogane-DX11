#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"
#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Effect/CEffectMovieClip.h"
#include "../Event/CEvent.h"
#include "../Victory/CVictory_change.h"

#include "CPhaseBall.h"
#include "CPhase_init.h"

namespace BMW{
namespace SLG{
namespace Phase{

CPhase_init::~CPhase_init()
{
	DELETE_SAFE(pPhaseCtrl_);
}

void CPhase_init::OnReset(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	pBall_ = p->getEvent()->getTurnBallPtr();

	Effect::CEffectMovieClip* pEffect;
	// フェーズエフェクト設定
	pPhaseCtrl_ = new Task::CTaskCtrl<Effect::CEffectMovieClip>;
	pEffect = p->getEffectDB().createEffect("PLAYER_PHASE");
	pPhaseCtrl_->addTask(pEffect,Phase::PLAYER);
	pEffect = p->getEffectDB().createEffect("ENEMY_PHASE");
	pPhaseCtrl_->addTask(pEffect,Phase::ENEMY);
	pEffect = p->getEffectDB().createEffect("NEUTRAL_PHASE");
	pPhaseCtrl_->addTask(pEffect,Phase::NEUTRAL);

	pPhaseCtrl_->valid(false);
	pPhaseCtrl_->visible(false);
}

namespace{

int nextPhase(CSLGContext* p)
{
	int nPhase;
	switch(p->getPhase())
	{
	case Phase::NEUTRAL:
		// これの時は一回りなので、ターン数を増やす
		nPhase = Phase::PLAYER;
		p->setTurn(p->getTurn()+1);
	break;

	case Phase::ENEMY:		nPhase = Phase::NEUTRAL;	break;
	default:				nPhase = Phase::ENEMY;		break;
	}
	p->setPhase(nPhase);
	p->phasePer();

	// 現在のフェイズにキャラがいるか？
	// いないなら、-1を返す
	return p->getPhaseList(nPhase).empty() ? -1 : nPhase;
}

} // namespace end

void CPhase_init::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// フェーズを切り替え
	int nPhase=-1;
	while(nPhase<0) nPhase=nextPhase(p);

	// 全キャラの状態をBEFOREにする
	// ↓これを呼び出すと各PhaseListが正常化される
	p->allActBefore();
	// フェーズに属するキャラのactionPhaseStartを呼び出す
	p->phaseStartPhase(nPhase);

	// ターンボール反映
	pBall_->OnReset(p);

	// フェーズ切り替えエフェクト
	pPhaseCtrl_->setState(nPhase);
	pPhaseCtrl_->getCurrentTask()->OnReset(p);
	pPhaseCtrl_->valid(true);
	pPhaseCtrl_->visible(true);

	setState(EFFECT);
}

void CPhase_init::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EFFECT:
		if(pBall_->getState()==CPhaseBall::NORMAL && pPhaseCtrl_->getCurrentTask()->IsEnd())
		{
			pPhaseCtrl_->valid(false);
			pPhaseCtrl_->visible(false);
			setState(PHASE_START);	
		}
	break;

	case PHASE_START:
		getTaskListCtrl()->callTaskList(Rule::PHASE_START,true);
	break;

	case VICTORY:
		getTaskListCtrl()->callTaskList(Rule::VICTORY_CHECK,true);
	break;

	case VICTORY_CHANGE:
		getTaskListCtrl()->callTaskList(Rule::VICTORY_CHANGE,true);
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CPhase_init::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::PHASE_START:
		// フェーズスタートイベント後は勝利条件チェック
		setState(VICTORY);
	break;

	case Rule::VICTORY_CHECK:
	// 勝利条件判定後
		switch(pContext->top())
		{
		case Victory::EXPERT:
		// 熟練度条件が成立
			pContext->pop();
			// 熟練度を+1にして、熟練度判定を無効にする
			pContext->setValue(pContext->getValue(Flag::EXPERT)+1,Flag::EXPERT);
			// 熟練度獲得をお知らせ
			pContext->push(Victory::CVictory_change::EXPERT_GET);
			pBall_->update(pContext);
			setState(VICTORY_CHANGE);
		break;

		default:
		// 特に成立していない
			setState(END);
		break;
		}
	break;

	case Rule::VICTORY_CHANGE:
	// 勝利条件が同時に設定されてるかもしれないので、
		// もう一回、チェック
		setState(VICTORY);
	break;

	default:
		setState(END);
	break;
	}
}

void CPhase_init::callTaskAction(Task::CTaskContext* pContext)
{
	pPhaseCtrl_->Task(pContext);
}

void CPhase_init::callTaskDraw(Task::CTaskContext* pContext)
{
	pPhaseCtrl_->Task(pContext);
}

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end