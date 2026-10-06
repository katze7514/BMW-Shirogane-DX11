#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"

#include "CAttack_rule2.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_rule2::OnInit(Task::CTaskContext* pContext)
{// 武器選択から
	setState(WEAPON);
	// 高さも無効にしておく
	pContext->push(-1);
	// 距離は無効にしておく
	pContext->push(-1); // 実距離
	pContext->push(-1);
	// 援護でもないっす
	pContext->push(0);
}

void CAttack_rule2::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case WEAPON:	getTaskListCtrl()->callTaskList(Rule::ATTACK_WEAPON,true);	break;
	case FIELD:		getTaskListCtrl()->callTaskList(Rule::FIELD_RULE,true);		break;
	case SELECT:	getTaskListCtrl()->callTaskList(Rule::ATTACK_SELECT,true);	break;
	case ACTION:	getTaskListCtrl()->callTaskList(Rule::ATTACK_ACTION,true);	break;
	case BATTLE_START: getTaskListCtrl()->callTaskList(Rule::BATTLE_START,true);break;
	case DEMO:		getTaskListCtrl()->callTaskList(Rule::ATTACK_DEMO,true);	break;
	case APPLY:		getTaskListCtrl()->callTaskList(Rule::ATTACK_APPLY,true);	break;
	case BATTLE_END: getTaskListCtrl()->callTaskList(Rule::BATTLE_END,true);	break;
	case DEL:		getTaskListCtrl()->callTaskList(Rule::ATTACK_DEL,true);		break;
	case RESULT:	getTaskListCtrl()->callTaskList(Rule::ATTACK_RESULT,true);	break;
	case HIT_AWAY:	getTaskListCtrl()->callTaskList(Rule::MENU_CHARA,true);		break;
	case WAIT:		getTaskListCtrl()->callTaskList(Rule::WAIT_RULE,true);		break;

	case CANCEL:
		getTaskListCtrl()->returnTaskList();
	break;

	case END:
		// こいつが呼ばれるということは、攻撃ルールが全部終了した
		// ということなので、0を積んでリターンする
		pContext->push(0);
		// 消していたカーソルを表示
		pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
		getTaskListCtrl()->returnTaskList();
	break;

	case WAIT_END:
		// 消していたカーソルを表示
		pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CAttack_rule2::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::ATTACK_WEAPON:
		if(pContext->top()>=0)
		{// 選択されれば、選択した武器IDがスタックトップに積まれる
			// それを操作対象武器として設定する
			pContext->setValue(pContext->top(), Flag::CTRL_WEAPON);
			pContext->pop();
			// 設定された武器がフィールド属性持ちだったらマップモードへ
			if(static_cast<CSLGContext*>(pContext)->getCtrlWeaponData()->IsF())
				setState(FIELD);
			else // そうじゃなければ、通常の対象選択
				setState(SELECT);
		}
		else
		{// キャンセルされたら、-1が積まれてるので、リターン
		 // -1をpopしないのは、MENU_CHARAに処理させるため
			static_cast<CSLGContext*>(pContext)->clearAttack();
			setState(CANCEL);
		}
	break;

	case Rule::ATTACK_SELECT:
		if(pContext->top()>=0)
		{// 選択されれば、選択した対象のSLG IDがスタックトップに積まれる
		 // その対象は、対象キャラに設定されている
			setState(ACTION);
			pContext->pop();
		}
		else
		{// キャンセルされたら、-1が積まれてる
			pContext->pop();
			// 高さも無効にしておく
			pContext->push(-1);
			// 距離は無効にしておく
			pContext->push(-1);
			pContext->push(-1);
			// もちろん、援護でもないっす
			pContext->push(0);
			// もう一度武器選択
			setState(WEAPON);
			// カーソル位置
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			p->getMap()->scrollIndex(p->getCtrlCharaData()->getIndex());
			p->setCirclePos(Pos::CHARA);
		}
	break;

	case Rule::ATTACK_ACTION:
		if(pContext->top()>=0)
		{// 決定されると、スタックトップには、デモのON(1)/OFFが積まれてる
		 // ただ、それは、フラグに設定されている
		 // んで、バトルスタートハンドラを呼び出す
			setState(BATTLE_START);
			// デモ中はカーソルいらないので非表示
			pContext->getInput()->cursolVisible(false);
		}
		else
		{// キャンセルされたら、-1が積まれてるので、もう一度範囲選択
			setState(SELECT);
		}
		pContext->pop();
	break;

	case Rule::BATTLE_START:
		setState(DEMO);
		//setState(APPLY);
	break;

	case Rule::ATTACK_DEMO:	 setState(APPLY);	break;

	case Rule::ATTACK_APPLY:
		// んで、バトル終了ハンドラを呼び出す
		setState(BATTLE_END);
	break;

	case Rule::BATTLE_END:
		setState(DEL);
	break;

	case Rule::ATTACK_DEL:
		setState(RESULT);
	break;

	case Rule::ATTACK_RESULT:
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		CDataCharaSLG* pChara = p->getCtrlCharaData();
	
		if(pChara->getState().getAct()==Act::BEFORE
		&& (   pChara->getBattle().hasSkill(Ability::HITAWAY)>=0
			|| (pChara->getBattle().IsTalent(Ability::TWICE_ATTACK) && p->getApp()->getAbility().enable(*pChara,0,*p,Ability::TWICE_ATTACK) )
			|| pChara->getBattle().IsTalent(Ability::TWICE_ATTACK_ENEMY)
			)
		)
		{// 移動後攻撃でない かつ (一撃離脱持ち or 二回攻撃持ち)
			setState(HIT_AWAY);
			pContext->getInput()->cursolVisible(true);
			pChara->getState().setAct(Act::HIT_AWAY);
		}
		else
		{	setState(WAIT);	}
	}
	break;

	case Rule::MENU_CHARA:
		// 一撃離脱後メニューがキャンセルできないことは、キャラメニューで保証
		pContext->getInput()->cursolVisible(false);
		if(pContext->top()!=2)	setState(END);
		else					setState(WAIT_END);
	break;

	case Rule::WAIT_RULE:
		setState(WAIT_END);
	break;

	case Rule::FIELD_RULE:
		if(pContext->top()<0)
		{// FIELD_RULEから戻ってきて、負が積まれてたらキャンセル
			pContext->pop();
			// 高さも無効にしておく
			pContext->push(-1);
			// 距離は無効にしておく
			pContext->push(-1);
			pContext->push(-1);
			// もちろん、援護でもないっす
			pContext->push(0);
			// もう一度武器選択
			setState(WEAPON);
			// カーソル位置
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			//static_cast<CSLGContext*>(pContext)->setCirclePos(Pos::CHARA);
			p->getMap()->scrollIndex(p->getCtrlCharaData()->getIndex());
			setState(WEAPON);
		}
		else
		{// 実行された後
			pContext->pop();
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			CDataCharaSLG* pChara = p->getCtrlCharaData();
	
			if(pChara->getState().getAct()==Act::BEFORE
			&& (pChara->getBattle().hasSkill(Ability::HITAWAY)>=0
				|| (    pChara->getBattle().IsTalent(Ability::TWICE_ATTACK)
					&&  p->getApp()->getAbility().enable(*pChara,0,*p,Ability::TWICE_ATTACK)
					)
				|| pChara->getBattle().IsTalent(Ability::TWICE_ATTACK_ENEMY)
			))
			{// 移動後攻撃でない かつ 一撃離脱・二回攻撃持ち
				setState(HIT_AWAY);
				pContext->getInput()->cursolVisible(true);
				pChara->getState().setAct(Act::HIT_AWAY);
			}
			else
			{
				setState(WAIT);	
			}
		}
	break;

	default: break;
	}
}

} // namesapce Attack end
} // namespace SLG end
} // namespace BMW end