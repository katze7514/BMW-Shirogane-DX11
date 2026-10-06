#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CDataBattle.h"

#include "CAttack_apply.h"

namespace BMW{
namespace SLG{
namespace Attack{

namespace{
__inline void killMental(CDataBattleBase& atk, CDataBattleBase def, CSLGContext& p)
{
	const smart_ptr<CDataCharaSLG>& attackChara = atk.getChara();
	const smart_ptr<CDataCharaSLG>& defChara = def.getChara();
	// 撃墜！
	attackChara->actionMental(Mental::FALL);
	// 仲間が撃墜したで～
	p.allMental(attackChara->getPhase(),Mental::FALL_FIREND, attackChara->getID());
	// 敵に撃墜された・・・
	p.allMental(defChara->getPhase(),Mental::FALL_ENEMY,defChara->getID());
}
} // namespace end

void CAttack_apply::OnAction(Task::CTaskContext* pContext)
{// 効果を適用する

	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 防御キャラ
	CDataCharaSLG* defChara=NULL;

	if(p->getCtrlWeaponData()->IsF())
	{// フィールド武器
		CDataBattleMap& battle = p->getBattleMap();
		applyMap(battle.getMapAtk(), *battle.currentMapDef(), *p);
		defChara = battle.currentMapDef()->getChara();

	}
	else
	{// MAP兵器じゃない
		// 戦闘データ取得
		smart_ptr<CDataBattle>& pBattle = p->getBattleData();

		// 攻撃側データ
		CDataBattleBase& attack			= pBattle->getBattleData(CDataBattle::ATTACK);
		// 援護攻撃データ
		CDataBattleBase& attackBack		= pBattle->getBattleData(CDataBattle::ATTACK_BACK);
		// 反撃側データ
		CDataBattleBase& counter		= pBattle->getBattleData(CDataBattle::COUNTER);
		defChara = counter.getChara().getPointer();
		// 援護防御
		CDataBattleBase& counterBack	= pBattle->getBattleData(CDataBattle::COUNTER_BACK);

		// 攻撃

		// 援護防御は発動？
		if(counterBack.getDefence().getAction()!=Battle::NO)
		{// 援護防御はあり
			apply(attack,counterBack,*p);
			// 援護回数を減らす
			counterBack.getChara()->getBattle().decDefence();
			// 倒された？
			if(attack.getAttack().IsDeath())
				killMental(attack,counterBack,*p);
		}
		else
		{// 援護防御はない
			apply(attack,counter,*p);
			// 倒された？
			if(attack.getAttack().IsDeath())
				killMental(attack,counter,*p);
		}

		// 反撃はあったか？
		if(attack.getDefence().getAction()!=Battle::NO)
		{
			apply(counter,attack,*p);
			// 倒された？
			if(counter.getAttack().IsDeath())
				killMental(counter,attack,*p);
		}

		// 援護攻撃はあったか？
		if(attackBack.getDefence().getAction()!=Battle::NO)
		{
			apply(attackBack,counter,*p,true);
			// 援護回数を減らす
			attackBack.getChara()->getBattle().decAttack();
			// 倒された？
			if(attackBack.getAttack().IsDeath())
				killMental(attack,counter,*p);
		}

	}

	// 防御側が行動不能だったら
	if(defChara!=NULL
	&& defChara->getBattle().IsCond(Chara::CValidCond::ACTION))
	{// フラグを3つ倒す
		defChara->getBattle().condDec(Chara::CValidCond::ACTION);
		defChara->getBattle().condDec(Chara::CValidCond::ACTION);
		defChara->getBattle().condDec(Chara::CValidCond::ACTION);
	}

	// 適用終了したので、リターン
	getTaskListCtrl()->returnTaskList();
}

void CAttack_apply::applyMap(CDataBattleMapAtk& atk, CDataBattleMapDef& def, CSLGContext& p)
{// MAP兵器
	// 武器使用によるEN消費
	// ↓ここにおいておくと複数回実行されてしまうので、別んとこでやる
	//atk.getWeapon()->use(*atk.getChara(),p);
	def.getChara()->getBattle().calcEN(def.getEnAbility());

	if(atk.getWeapon()->getKind()==Weapon::Kind::STATUS)
	{// ステータスアップだったら効果適用
		atk.getWeapon()->apply(*def.getChara(),p);
	}
	else
	{
		// 攻撃が当たってればダメージ
		// 状態変化だったら
		if(def.IsHit()
		&& def.getDamage()>0
		&& (atk.getWeapon()->getKind()==Weapon::Kind::FIGHT_COND || atk.getWeapon()->getKind()==Weapon::Kind::MAGIC_COND))
		{// その効果を適用
			atk.getWeapon()->apply(*def.getChara(),p);
		}
		// ダメージの適用
		def.getChara()->calcHP(def.getDamage());
		// HPが0以下だったら死亡
		// すでにEVENT系ACTになってたら何もしない
		if(def.IsDeath() && def.getChara()->getState().getAct()!=Act::DEATH_EVENT)
			def.getChara()->getState().setAct(Act::DEATH);
	}

	// 気力
	if(atk.getWeapon()->getKind()==Weapon::Kind::STATUS) return;

	if(def.IsHit()
	&& def.getDamage()>0)
	{
		// 攻撃が命中
		atk.getChara()->actionMental(Mental::HIT);
		// 攻撃側、意気揚々持ってる？
		if(atk.getChara()->getBattle().hasSkill(Ability::IKIYOYO)>=0)
			p.getApp()->getAbility().applyStatus(*atk.getChara(),0,Ability::IKIYOYO);
		
		// 攻撃を受けた
		def.getChara()->actionMental(Mental::DAMAGE);
		// 防御側は、対抗心を持ってる？
		if(def.getChara()->getBattle().hasSkill(Ability::AGAINST)>=0)
			p.getApp()->getAbility().applyStatus(*def.getChara(),0,Ability::AGAINST);
		// 撃墜したか
		if(def.IsDeath())
		{// 撃墜！
			atk.getChara()->actionMental(Mental::FALL);
			// 仲間が撃墜したで～
			p.allMental(atk.getChara()->getPhase(),Mental::FALL_FIREND, atk.getChara()->getID());
			// 敵に撃墜された・・・
			p.allMental(def.getChara()->getPhase(),Mental::FALL_ENEMY,def.getChara()->getID());
		}
	}
	else
	{
		// 攻撃をはずした もしくは、無効化された
		atk.getChara()->actionMental(Mental::MISS);
		// 攻撃を避けた もしくは、無効化した
		def.getChara()->actionMental(Mental::AVOID);
		// 防御側は、気力+回避を持ってる？
		if(def.getChara()->getBattle().hasSkill(Ability::RITHM)>=0)
			p.getApp()->getAbility().applyStatus(*def.getChara(),0,Ability::RITHM);
	}
}

void CAttack_apply::apply(CDataBattleBase& attack, CDataBattleBase& def, CSLGContext& p, bool bAttBack)
{// ようはattack→defへのダメージ等の適用
	CDataBattleAttack& att = attack.getAttack();
	CDataBattleDefence& de  = bAttBack ? attack.getDefence() : def.getDefence();

	smart_ptr<Weapon::CDataWeaponBattle>& pWeapon = attack.getAttack().getWeaponData();
	// 武器使用によるEN消費
	pWeapon->use(*attack.getChara(),p);
	// 技能EN
	attack.getChara()->getBattle().calcEN(att.getEnAbility());
	def.getChara()->getBattle().calcEN(de.getEnAbility());
	
	// ステータス変化だったら
	if(pWeapon->getKind()==Weapon::Kind::STATUS)
	{// その効果を適用
		pWeapon->apply(*def.getChara().getPointer(),p);
	}
	else
	{
		// 攻撃当たって状態変化武器だったら
		if(attack.getAttack().getDamage()>0
		&& (pWeapon->getKind()==Weapon::Kind::FIGHT_COND || pWeapon->getKind()==Weapon::Kind::MAGIC_COND))
		{// その効果を適用
			pWeapon->apply(*def.getChara().getPointer(),p);
		}
		// ダメージの適用
		def.getChara()->calcHP(attack.getAttack().getDamage());
		// HPが0以下だったら死亡
		// すでにEVENT系ACTになってたら何もしない
		if(att.IsDeath() && def.getChara()->getState().getAct()!=Act::DEATH_EVENT)
			def.getChara()->getState().setAct(Act::DEATH);
	}
	// 気力
	// 回復・ステータス系の時は必要無し
	if(pWeapon->getKind()==Weapon::Kind::CURE
	|| pWeapon->getKind()==Weapon::Kind::REFILL
	|| pWeapon->getKind()==Weapon::Kind::STATUS)
		return;

	// 攻撃命中に関する気力
	if(de.getAction()!=Battle::AVOID
	&& att.getDamage()>0)
	{// 攻撃が命中
		attack.getChara()->actionMental(Mental::HIT);
		// 攻撃側は、意気揚々を持ってる？
		if(attack.getChara()->getBattle().hasSkill(Ability::IKIYOYO)>=0)
			p.getApp()->getAbility().applyStatus(*attack.getChara(),0,Ability::IKIYOYO);

		// 攻撃を受けた
		def.getChara()->actionMental(Mental::DAMAGE);
		// 防御側は、対抗心を持ってる？
		if(def.getChara()->getBattle().hasSkill(Ability::AGAINST)>=0)
			p.getApp()->getAbility().applyStatus(*def.getChara(),0,Ability::AGAINST);
	}
	else
	{
		// 攻撃をはずした もしくは、無効化された
		attack.getChara()->actionMental(Mental::MISS);
		// 攻撃を避けた もしくは、無効化した
		def.getChara()->actionMental(Mental::AVOID);
		// 防御側は、リズム感を持ってる？
		if(def.getChara()->getBattle().hasSkill(Ability::RITHM)>=0)
			p.getApp()->getAbility().applyStatus(*def.getChara(),0,Ability::RITHM);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end