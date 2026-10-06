#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Map/CMap.h"
#include "../Event/CEvent.h"
#include "../Phase/CPhaseBall.h"
#include "../Action/CActionNormal.h"
#include "../Victory/CVictory_change.h"

#include "CMenu_select_cpu.h"

namespace BMW{
namespace SLG{
namespace Menu{

namespace{
__inline list<int>& getPhaseList(CSLGContext* p)
{
	if(p->getValue(Flag::PHASE)==Phase::PLAYER)
	{// Playerフェーズの時はNonPlayerListを返す
		return p->getNonPlayerPhaseList();
	}
	else // それ以外
		return p->getPhaseList(p->getValue(Flag::PHASE));
		
}
} // namespace end

void CMenu_select_cpu::OnInit(Task::CTaskContext* pContext)
{// 現在のフェーズから、操作対象のイテレータを設定する
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	p->setEnemyReset(0);
	it=getPhaseList(p).begin();
	actionSelect(p);

	nActionCount_=0;
}

void CMenu_select_cpu::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CHANGE:
	{	// 行動回数が終了してたら、イテレータを進める
		if(nActionCount_<=0) ++it;

		actionSelect(static_cast<CSLGContext*>(pContext));
	}
	break;

	case EXEC:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 操作キャラを設定
		p->setCtrlChara(*it);
		CDataCharaSLG* pChara = p->getCtrlCharaData();
		if(pChara==NULL
		|| !pChara->IsExist())
		{// キャラが不正、MAPにいないなら削除
			p->delCharaData(*it);
			it = p->delPhase(*it,p->getPhase(),p->getPhase()==Phase::PLAYER);

			nActionCount_=0;
			actionSelect(p);
		}
		ef(pChara->getIndex()<0
		|| pChara->getState().getAct()==Act::AFTER)
		{// マップに存在しない、行動済み
		 //	だったら次へ
			nActionCount_=0;
			setState(CHANGE);
		}
		else
		{
			pAction_ = static_cast<Action::CActionNormal*>(p->getCtrlCharaData()->getAction());
			// マップをそいつを中央に移動する
			p->getMap()->scrollIndex(p->getCtrlCharaData()->getIndex());
			// ↑を反映させるために一度から回し
			setState(ACTION);

			// キャラが変わっていて二回行動持ってる？
			if(nActionCount_<=0)
			{
				if(pChara->getBattle().IsTalent(Ability::TWICE_ACTION))
				{	nActionCount_=2; }
				else
				{	nActionCount_=1; }
				// 二回攻撃持ち？
				bTwiceAttack_ = pChara->getBattle().IsTalent(Ability::TWICE_ATTACK_ENEMY);
			}
		}
	}
	break;

	case ACTION:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 思考ルーチン起動
		pAction_->action(*p->getCtrlCharaData(),*p);

		// 各種フラグに設定
		p->setCtrlWeapon(pAction_->getUseWeapon());
		p->setTargetChara(pAction_->getTargetChara());
		p->setTargetMap(pAction_->getMapIndex());

		// メモ
		// もし、敵に一撃離脱を持たせるなら↑のようにするとダメ
		// ATTACK_RULE_CPUで、攻撃対象のマップにTargetMapが書き換えられている

#ifdef BMW_DEBUG
		CDbg().Out("EnemyAction %d %d %d %d %d %d %d",p->getCtrlCharaData()->getIndex()
											,pAction_->getUseWeapon()
											,pAction_->getTargetChara()
											,pAction_->getMapIndex()
											,nActionCount_
											,bTwiceAttack_
											,p->getCtrlCharaData()->getState().getAct()
											);
#endif


		// 計算状況に合わせて、行動する
		if(pAction_->getMapIndex()>=0)	setState(MOVE);
		ef(pAction_->getUseWeapon()>=0) setState(ATTACK);
		else{ nFrame_=0; setState(PRE_WAIT); }
	}
	break;

	case CENTER:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// マップをそいつを中央に移動する
		p->getMap()->scrollIndex(p->getCtrlCharaData()->getIndex());

		if(p->getCtrlWeapon()>=0)
		{// 武器が設定されてば、そのまま攻撃へ
			setState(ATTACK);
		}
		else
		{// されてなければ、WAIT
			setState(PRE_WAIT);
			nFrame_=0;
		}
	}
	break;

	case MOVE:				getTaskListCtrl()->callTaskList(Rule::MOVE_RULE_CPU,true);		break;
	case ATTACK:			getTaskListCtrl()->callTaskList(Rule::ATTACK_RULE_CPU,true);	break;

	case PRE_WAIT:	if(nFrame_++>10) setState(WAIT); break;

	case WAIT:				getTaskListCtrl()->callTaskList(Rule::WAIT_RULE,true);			break;
	case CHARA_END:			getTaskListCtrl()->callTaskList(Rule::CHARA_END,true);			break;
	case VICTORY:			getTaskListCtrl()->callTaskList(Rule::VICTORY_CHECK,true);		break;
	case VICTORY_CHANGE:	getTaskListCtrl()->callTaskList(Rule::VICTORY_CHANGE,true);		break;
	case PHASE_END:			getTaskListCtrl()->callTaskList(Rule::PHASE_END,true);	break;
	case END:				getTaskListCtrl()->returnTaskList();	break;

	default: break;
	}
}

void CMenu_select_cpu::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	// エネミーイテレーションリセットフラグが立ってるかもしれない
	if(nID!=Rule::ATTACK_RULE_CPU
	&& pContext->getValue(Flag::ENEMY_RESET))
	{// 立ってたらリセット
		pContext->setValue(0,Flag::ENEMY_RESET);
		it=getPhaseList(static_cast<CSLGContext*>(pContext)).begin();
	}

	switch(nID)
	{
	case Rule::MOVE_RULE_CPU:
	{// 移動が終わった
	 //	一度、表示をセンターへ
		setState(CENTER);
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		p->clearMove();
	}
	break;

	case Rule::ATTACK_RULE_CPU:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// 一応、攻撃範囲クリア
		p->clearAttack();
	
		if(p->getCtrlCharaData()->IsLive())
		{// 攻撃が終わった生きてれば、WAITする
			// 立ち止まり攻撃だったら、2回攻撃をするかもしれない
			if(bTwiceAttack_
			&& p->getCtrlCharaData()->getState().getAct()!=Act::MOVE // 移動後に設定されてたら二回目の攻撃
			&& pAction_->getMapIndex()<0
			&& pAction_->getUseWeapon()>=0
			)
			{// 2回攻撃が有効
				CSLGContext* p = static_cast<CSLGContext*>(pContext);
				// 二回目の攻撃だということで移動後にしておく
				p->getCtrlCharaData()->getState().setAct(Act::MOVE);
				// そしてもう一度実行
				setState(EXEC);
			}
			else
			{// そうじゃなければWAIT
				setState(WAIT);
			}
		}
		else
		{// 死んでたり離脱してたら、キャラEndを呼び出す
			setState(CHARA_END);
		}
	}
	break;

	case Rule::WAIT_RULE:
	{// Waitから戻ってきたら、キャラエンドへ
	 // ただ、スタックに値が一個積まれている
		pContext->pop();
		setState(CHARA_END);

		if(--nActionCount_>0) // 行動回数を減らす
		{// 減らしてもまだ回数が残ってたら、もう一度
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			p->getCtrlCharaData()->getState().setAct(Act::BEFORE);
		}
	}
	break;

	case Rule::CHARA_END:
		// キャラエンド後には、勝利条件成立フラグ立ってるかもしれない
		// ので、勝利条件判定
		setState(VICTORY);
	break;

	case Rule::VICTORY_CHECK:
		switch(pContext->top())
		{
		case Victory::VICTORY:
		// 勝利条件が成立
		case Victory::LOSE:
		// 敗北条件が成立
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
		{// 特に成立していない
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			p->pop();
			// 現在、操作中のキャラ状態に合わせて、処理を振り分ける
			if(p->getCtrlCharaData()->IsExist())
			{// 死んでなかったら、次のキャラ
				setState(CHANGE);
			}
			else
			{// 死んでたら
				actionSelect(p);
			}
		}
		break;
		}
	break;

	case Rule::VICTORY_CHANGE:
		// ついでに、勝利条件が同時に設定されてるかもしれないので、
		// もう一回、チェック
		setState(VICTORY);
	break;

	case Rule::PHASE_END:
	// PHASE_ENDから戻ってきたら、終了
		pContext->push(Victory::NO);
		setState(END);
	break;

	default: break;
	}
}

void CMenu_select_cpu::actionSelect(CSLGContext* p)
{
	if(it==getPhaseList(p).end())
	{// 全キャラ消化してたら、フェーズ終了
		setState(PHASE_END);
	}
	else
	{// 特になにもなければ実行
		setState(EXEC);
	}
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end