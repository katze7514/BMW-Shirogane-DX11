#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/COffsetWeapon.h"

#include "../Map/CMap.h"
#include "../Map/CMapChipState.h"

#include "CAttack_range2.h"
#include "CAttack_rule_cpu.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_rule_cpu::OnInit(Task::CTaskContext* pContext)
{
	// まずは、攻撃範囲計算から
	setState(RANGE);

	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 選択された状況を設定
	Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();
	// フィールド武器
	bMap_ = pWeapon->IsF();
	// 攻撃対象のマップをセットしておく
	if(!bMap_) p->setTargetMap(p->getTargetCharaData()->getIndex());
}

void CAttack_rule_cpu::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case RANGE: getTaskListCtrl()->callTaskList(Rule::ATTACK_RANGE, true); break;
	case VIEW:
		// 攻撃カーソル移動
		if(++nWait_>=15)
		{// 終わったら、ACTION表示
			Map::CMapChipState::attack(false);
			// MAP武器だったら、MAP_RULEへ
			if(bMap_)
				setState(FIELD_RULE);
			else	
				setState(ACTION);
		}
	break;

	case FIELD_RULE:	getTaskListCtrl()->callTaskList(Rule::FIELD_RULE, true); break;
	case ACTION:		getTaskListCtrl()->callTaskList(Rule::ATTACK_ACTION, true); break;
	case BATTLE_START:	getTaskListCtrl()->callTaskList(Rule::BATTLE_START, true);	break;
	case DEMO:			getTaskListCtrl()->callTaskList(Rule::ATTACK_DEMO, true); break;
	case APPLY:			getTaskListCtrl()->callTaskList(Rule::ATTACK_APPLY, true);	break;
	case BATTLE_END:	getTaskListCtrl()->callTaskList(Rule::BATTLE_END, true);	break;
	case DEL:			getTaskListCtrl()->callTaskList(Rule::ATTACK_DEL,true);		break;
	case RESULT:		getTaskListCtrl()->callTaskList(Rule::ATTACK_RESULT, true); break;
	case END:			getTaskListCtrl()->returnTaskList();						break;
	default: break;
	}
}

void CAttack_rule_cpu::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::ATTACK_RANGE:
		// 攻撃可能範囲設定
		// フィールド武器の場合は、攻撃方向はすでに設定されている
		Map::CMapChipState::setRangeData(static_cast<CSLGContext*>(pContext)->getCtrlWeaponData(), bMap_ ? -2 : -1);
		// 攻撃範囲表示
		Map::CMapChipState::attack(true);
		Map::CMapChipState::mapValid(true);
		// 攻撃カーソル移動初期設定
		nWait_=0;
		setState(VIEW);
	break;

	case Rule::FIELD_RULE:
		// マップルールから戻ってきたら終了
		pContext->pop();
		setState(END);
	break;

	case Rule::ATTACK_ACTION:
		// じゃ、次
		pContext->pop();
		pContext->getInput()->cursolVisible(false);
		setState(BATTLE_START);
	break;

	case Rule::BATTLE_START: setState(DEMO);		break;
	case Rule::ATTACK_DEMO: setState(APPLY);		break;
	case Rule::ATTACK_APPLY: setState(BATTLE_END);	break;
	case Rule::BATTLE_END:	setState(DEL);			break;
	case Rule::ATTACK_DEL:	setState(RESULT);		break;
	case Rule::ATTACK_RESULT: setState(END);		break;

	default: break;
	}
}

} // namespace Attack end
} // namespace SLG end
} // namesapce BMW end