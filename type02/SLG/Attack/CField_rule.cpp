#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Demo/CDemo_map.h"

#include "CField_rule.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CField_rule::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	if(p->getCtrlCharaData()->getPhase()!=Phase::PLAYER)
	{// 敵だったら、ATTACKから
		setState(ATTACK);
	}
	else
	{// 範囲選択から
		setState(SELECT);
	}
}

void CField_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case SELECT:		getTaskListCtrl()->callTaskList(Rule::FIELD_SELECT,true);	break;
	case ATTACK:		getTaskListCtrl()->callTaskList(Rule::FIELD_ATTACK,true);	break;
	case BATTLE_START:	getTaskListCtrl()->callTaskList(Rule::BATTLE_START,true);	break;

	case EFFECT:	
		if(pContext->getValue(Flag::DEMO)) // デモONの時はATTACK_DEMOを呼び出す
			getTaskListCtrl()->callTaskList(Rule::ATTACK_DEMO,true);
		else // OFFだったら、終わったことにしてすぐ次へ
			OnComeBack(Rule::ATTACK_DEMO, pContext);
	break;

	case DEMO:
		pContext->push(Demo::CDemo_map::BATTLE);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	break;

	case APPLY:			getTaskListCtrl()->callTaskList(Rule::ATTACK_APPLY,true);	break;
	case BATTLE_END:	getTaskListCtrl()->callTaskList(Rule::BATTLE_END,true);		break;
	case DEL:			getTaskListCtrl()->callTaskList(Rule::ATTACK_DEL,true);		break;
	case RESULT:		getTaskListCtrl()->callTaskList(Rule::ATTACK_RESULT,true);	break;
	case END:			getTaskListCtrl()->returnTaskList();						break;
	default: break;
	}
}

void CField_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::FIELD_SELECT:
		if(pContext->top()>=0)
		{// 選択されれば、選択した対象のSLG IDがスタックトップに積まれる
		 // その対象は、対象キャラに設定されている
			pContext->pop();
			setState(ATTACK);
		}
		else
		{// キャンセルされたら、リターンする
			setState(END);
		}
	break;

	case Rule::FIELD_ATTACK:

		if(pContext->top()>=0) // OK先へ
			setState(BATTLE_START);
		else // キャンセルされた選択へ戻る
			setState(SELECT);

		pContext->pop();
	break;

	case Rule::BATTLE_START:	setState(EFFECT);		break;

	case Rule::ATTACK_DEMO:
	//case Rule::MAP_EFFECT:
	{
		setState(DEMO);
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		CDataBattleMap& map = p->getBattleMap();
		// 武器消費
		CDataBattleMapAtk& atk = map.getMapAtk();
		atk.getWeapon()->use(*atk.getChara(),*p);
		// いろいろここから始まる
		map.beginMapDef();
	}
	break;

	case Rule::DEMO_MAP:		setState(APPLY);		break;
	case Rule::ATTACK_APPLY:	setState(BATTLE_END);	break;
	case Rule::BATTLE_END:		setState(DEL);			break;

	case Rule::ATTACK_DEL:
	{// 一人ずつマップデモを適用していく
		CDataBattleMap& map = static_cast<CSLGContext*>(pContext)->getBattleMap();
		map.nextMapDef();
		if(map.endMapDef())	// 処理するキャラがいなくなったらレザルト
			setState(RESULT);
		else	// まだ、いるならデモへ
			setState(DEMO);
	}
	break;

	case Rule::ATTACK_RESULT: pContext->push(0); setState(END);	break;
	default: break;
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end