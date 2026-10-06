#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Sally/CSally_del_chara_map.h"

#include "CAttack_del.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_del::OnInit(Task::CTaskContext* pContext)
{// 削除予定キャラリスト構築
	listDeath_.clear();

	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	if(p->getCtrlWeaponData()->IsF())
	{// MAP兵器
		CDataBattleMap& battle = p->getBattleMap();
		CDataBattleMapDef& def = *battle.currentMapDef();
		
		if(!def.getChara()->IsExist() && def.IsDeath())
			listDeath_.push_back(def.getChara()->getID());
	}
	else
	{// 通常兵器
		CDataBattleBase& attack = p->getBattleData()->getBattleData(CDataBattle::ATTACK);
		CDataBattleBase& counter = p->getBattleData()->getBattleData(CDataBattle::COUNTER);
		CDataBattleBase& attackBack = p->getBattleData()->getBattleData(CDataBattle::ATTACK_BACK);
		CDataBattleBase& counterBack = p->getBattleData()->getBattleData(CDataBattle::COUNTER_BACK);

		if(attack.getAttack().IsDeath())
		{// 攻撃側が誰かを倒してる
			// 援護防御は？
			if(counterBack.getDefence().getAction()!=Battle::NO)
			{// ありますね
				// すでにいなかったら、気にしない
				if(!counterBack.getChara()->IsExist())
					listDeath_.push_back(counterBack.getChara()->getID());
			}
			else
			{// 無いですね
			#ifdef BMW_DEBUG
				CDbg().Out("DEL ACT %d",counter.getChara()->getState().getAct());
			#endif
				if(!counter.getChara()->IsExist())
					listDeath_.push_back(counter.getChara()->getID());
			}
		}

		if(counter.getAttack().IsDeath())
		{// 反撃側が！
			if(!attack.getChara()->IsExist())
				listDeath_.push_back(attack.getChara()->getID());
		}

		if(attackBack.getAttack().IsDeath())
		{// 援護攻撃～
			//CDbg().Out("ACT %d",counter.getChara()->getState().getAct());
			if(!counter.getChara()->IsExist())
				listDeath_.push_back(counter.getChara()->getID());
		}
	}

	if(!listDeath_.empty())
	{// 削除対象があれば、削除
		it = listDeath_.begin();
		setState(DEL);
	}
	else // なければ、このまま終了
		setState(END);
}

void CAttack_del::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DEL:
		//CDbg().Out("DEL %d",*it);
		pContext->setValue(*it,Flag::TARGET_CHARA);
		pContext->push(Sally::CSally_del_chara_map::DEATH);
		getTaskListCtrl()->callTaskList(Rule::DEL_CHARA_MAP,true);
		setState(NORMAL);
	break;

	case END:
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CAttack_del::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::DEL_CHARA_MAP)
	{// キャラのマップ削除から戻ってきた
		if(++it == listDeath_.end())	setState(END);
		else							setState(DEL);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end
