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

#include "CCure_rule.h"

namespace BMW{
namespace SLG{
namespace Cure{

void CCure_rule::OnReset(Task::CTaskContext* pContext)
{
}

void CCure_rule::OnInit(Task::CTaskContext* pContext)
{
	// 治癒武器IDをCtrlWeaponに設定
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->setCtrlWeapon(p->getCtrlCharaData()->getBattle().IsCure());

	Map::CMapChipState::setRangeData(p->getCtrlWeaponData());

	setState(SELECT);
}

void CCure_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case SELECT:
		getTaskListCtrl()->callTaskList(Rule::ATTACK_SELECT,true);
	break;

	case DEMO:
	{// 回復量を計算して、デモを呼び出す
		pContext->push(Demo::CDemo_map::CURE);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	}
	break;

	case APPLY:
		getTaskListCtrl()->callTaskList(Rule::ATTACK_APPLY,true);
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

void CCure_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{

	case Rule::ATTACK_SELECT:
	if(pContext->top()>=0)
	{// スタックトップが0以上だったら、APPLYへ
		pContext->pop();
		setState(DEMO);
		// 回復量計算
		calcCure(pContext);
	}
	else
	{// 負だったらキャンセルされたので、キャラメニューへ
		setState(END);
	}
	break;

	case Rule::DEMO_MAP:
		setState(APPLY);
	break;

	case Rule::ATTACK_APPLY:
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

void CCure_rule::calcCure(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->getBattleData()->clearBattleData();

	// 回復を行うキャラ設定
	CDataBattleBase& cure = p->getBattleData()->getBattleData(CDataBattle::ATTACK);
	cure.setChara(smart_ptr<CDataCharaSLG>(p->getCtrlCharaData(),false));
	// 回復武器の設定
	cure.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(p->getCtrlWeaponData(),false));
	// 回復量の計算
	int nCure = 500+60*cure.getChara()->getBattle().getLv();

	// 回復対象キャラの設定
	CDataBattleBase& cureTarget = p->getBattleData()->getBattleData(CDataBattle::COUNTER);
	cureTarget.setChara(smart_ptr<CDataCharaSLG>(p->getTargetCharaData(),false));

	// 回復対象キャラの残りHPと比較して実回復量を調整
	int nRest = cureTarget.getChara()->getBattle().getMaxHP()-cureTarget.getChara()->getBattle().getHP();
	// 残りHPが回復量より小さいなら、回復値は残りHP分
	if(nCure>nRest) nCure=nRest;

	// 回復量の設定
	// 回復は負のダメージ扱い
	cure.getAttack().setDamage(-nCure);
}

} // namespace Cure end
} // namespace SLG end
} // namespace BMW end