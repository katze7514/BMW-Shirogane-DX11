#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChipState.h"

#include "../Demo/CDemo_map.h"

#include "CRefill_rule.h"

namespace BMW{
namespace SLG{
namespace Refill{

void CRefill_rule::OnReset(Task::CTaskContext* pContext)
{
}

void CRefill_rule::OnInit(Task::CTaskContext* pContext)
{
	// 補給武器IDをCtrlWeaponに設定
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->setCtrlWeapon(p->getCtrlCharaData()->getBattle().IsRefill());

	Map::CMapChipState::setRangeData(p->getCtrlWeaponData());

	setState(SELECT);
}

void CRefill_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case SELECT:
		getTaskListCtrl()->callTaskList(Rule::ATTACK_SELECT,true);
	break;

	case DEMO:
	{
		pContext->push(Demo::CDemo_map::REFILL);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	}
	break;

	case APPLY:
		getTaskListCtrl()->callTaskList(Rule::REFILL_APPLY,true);
	break;

	case RESULT:
		getTaskListCtrl()->callTaskList(Rule::ATTACK_RESULT,true);
	break;

	case WAIT:
		getTaskListCtrl()->callTaskList(Rule::WAIT_RULE,true);
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CRefill_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::ATTACK_SELECT:
	if(pContext->top()>=0)
	{// スタックトップが0以上だったら、APPLYへ
		pContext->pop();
		setState(DEMO);
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		p->getBattleData()->clearBattleData();
		// 補給を行うキャラ設定
		CDataBattleBase& refill = p->getBattleData()->getBattleData(CDataBattle::ATTACK);
		refill.setChara(smart_ptr<CDataCharaSLG>(p->getCtrlCharaData(),false));
		// 補給武器の設定
		refill.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(p->getCtrlWeaponData(),false));
		// 補給対象キャラの設定
		CDataBattleBase& refillTarget = p->getBattleData()->getBattleData(CDataBattle::COUNTER);
		refillTarget.setChara(smart_ptr<CDataCharaSLG>(p->getTargetCharaData(),false));
	}
	else
	{// 負だったらキャンセルされたので、キャラメニューへ
		setState(END);
	}
	break;

	case Rule::DEMO_MAP:
		setState(APPLY);
	break;

	case Rule::REFILL_APPLY:
		// APPLYが終了したら、RESULTする
		setState(RESULT);
	break;

	case Rule::ATTACK_RESULT:
		setState(WAIT);
	break;

	case Rule::WAIT_RULE:
		setState(END);
	break;

	default: break;
	}
}

} // namespace Refill end
} // namespace SLG end
} // namespace BMW end