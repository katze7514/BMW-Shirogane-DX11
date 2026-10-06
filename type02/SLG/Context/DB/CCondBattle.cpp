#include "stdafx.h"

#include "../../../Weapon/IDWeapon.h"
#include "../../../Weapon/CDataWeaponBattle.h"
#include "../CSLGContext.h"
#include "../CDataCharaSLG.h"

#include "CCondBattle.h"

namespace BMW{
namespace SLG{

bool CCondBattle::judg(CSLGContext* p)
{
#ifdef BMW_DEBUG
	CDbg().Out("BATTLE %d",getChara());
#endif

	switch(getKind())
	{
	case CHARA:
		if(getChara()==p->getCtrlChara()) return true;

		if(p->getCtrlWeaponData()->IsF()) // マップ兵器ならばこっち
			return getChara()==p->getBattleMap().currentMapDef()->getChara()->getID();
		else // 通常の武器ならこっち
			return getChara()==p->getTargetChara();
	break;

	case DAMAGE:
	{
		// くらったダメージ量
		int nDamage=0;
		if(p->getCtrlWeaponData()->IsF())
		{// マップ兵器ならばこっち
			CDataBattleMap& map = p->getBattleMap();
			// 被害者ならダメージ
			if(getChara()==map.currentMapDef()->getChara()->getID())
				nDamage = map.currentMapDef()->getDamage();
		}
		else
		{
			smart_ptr<CDataBattle>& battle = p->getBattleData();
			// まずは、キャラがどの場所にいるか判定
			// 攻撃キャラ？
			if(getChara()==p->getCtrlChara())
			{// 反撃があれば
				if(battle->getBattleData(CDataBattle::ATTACK).getDefence().getAction()!=Battle::NO)
					nDamage = battle->getBattleData(CDataBattle::COUNTER).getAttack().getDamage();
			}
			ef(getChara()==p->getTargetChara())
			{// 攻撃をくらった！？　ただし、援護あればセーフ
				if(battle->getBattleData(CDataBattle::COUNTER).getDefence().getAction()!=Battle::NO
				&& battle->getBattleData(CDataBattle::COUNTER_BACK).getDefence().getAction()==Battle::NO)
					nDamage = battle->getBattleData(CDataBattle::ATTACK).getAttack().getDamage();

				// 援護攻撃は？
				if(battle->getBattleData(CDataBattle::ATTACK_BACK).getDefence().getAction()!=Battle::NO)
					nDamage += battle->getBattleData(CDataBattle::ATTACK_BACK).getAttack().getDamage();
			}
			else
			{
				// 援護防御に入ってたよ
				CDataBattleBase& base = battle->getBattleData(CDataBattle::COUNTER_BACK);
				if(base.getDefence().getAction()!=Battle::NO
				&& getChara()==base.getChara()->getID())
					nDamage = battle->getBattleData(CDataBattle::ATTACK).getAttack().getDamage();
			}
		}
		// 判定ー
		// とりあえず、以上判定だけ
		return getValue() <= nDamage;
	}	
	break;

	default: return false;
	}
}

} // namespace SLG end
} // namespace BMW end