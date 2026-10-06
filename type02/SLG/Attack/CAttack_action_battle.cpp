#include "stdafx.h"

#include "../../Chara/CValidSpirit.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/ConstWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Ability/Ability/CAbility_Magicrelease.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"

#include "CAttack_calc.h"

#include "CAttack_action.h"

namespace BMW{
namespace SLG{
namespace Attack{

namespace{
__inline void flagDownSpirit(CDataCharaSLG &chara, bool bHit, bool bFriend)
{
	if(bHit)
	{// 当たった場合のみ
		// 魂
		if(chara.getBattle().IsSpirit(Chara::CValidSpirit::SPIRIT))
			chara.getBattle().spirit(false,Chara::CValidSpirit::SPIRIT);
		// 熱血
		ef(chara.getBattle().IsSpirit(Chara::CValidSpirit::FIREBALL))
			chara.getBattle().spirit(false,Chara::CValidSpirit::FIREBALL);
	}

	// てかげんのフラグを倒す
	chara.getBattle().spirit(false,Chara::CValidSpirit::EASYON);

	// 直撃フラグを倒す
	if(!bFriend) 
		chara.getBattlePtr()->spirit(false,Chara::CValidSpirit::DIRECT);

	// 狙撃フラグを倒す
	chara.getBattlePtr()->spirit(false,Chara::CValidSpirit::SNIPE);
}
} // namespace end

void CAttack_action::actionGo(Task::CTaskContext* pContext)
{
#ifdef BMW_DEBUG
	// Stateデータダンプ
	//CDbg().Out("ATTACK %d %d %d",state_.getCharaData(CBattleState::ATTACK)->getID(),
	//							state_.getWeaponID(CBattleState::ATTACK),
	//							state_.getHit(CBattleState::ATTACK));
	//CDbg().Out("A_BACK %d %d %d",state_.getCharaData(CBattleState::ATTACK_BACK)==NULL
	//							? -1 : state_.getCharaData(CBattleState::ATTACK_BACK)->getID(),
	//							state_.getWeaponID(CBattleState::ATTACK_BACK),
	//							state_.getHit(CBattleState::ATTACK_BACK));

	//CDbg().Out("COUNTER %d %d %d",state_.getCharaData(CBattleState::COUNTER)->getID(),
	//							state_.getWeaponID(CBattleState::COUNTER),
	//							state_.getHit(CBattleState::COUNTER));
	//CDbg().Out("C_BACK %d %d %d",state_.getCharaData(CBattleState::COUNTER_BACK)==NULL
	//							? -1 : state_.getCharaData(CBattleState::COUNTER_BACK)->getID(),
	//							state_.getWeaponID(CBattleState::COUNTER_BACK),
	//							state_.getHit(CBattleState::COUNTER_BACK));
#endif
	
	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 戦闘データ
	smart_ptr<CDataBattle>& pBattle = p->getBattleData();
	pBattle->clearBattleData(); // とりあえず、クリア
	release_.reset();
	nRelease_ = -1;

	// キャラデータだけは、ここで設定する
	// 攻撃
	CDataBattleBase& attack = pBattle->getBattleData(CDataBattle::ATTACK);
	CDataCharaSLG* pAttack = state_.getCharaData(CBattleState::ATTACK);
	// キャラデータの設定
	attack.setChara(smart_ptr<CDataCharaSLG>(pAttack,false));
	
	// 反撃
	CDataBattleBase& counter = pBattle->getBattleData(CDataBattle::COUNTER);
	CDataCharaSLG* pCounter = state_.getCharaData(CBattleState::COUNTER);
	// キャラデータの設定
	counter.setChara(smart_ptr<CDataCharaSLG>(pCounter,false));
	
	// 攻撃側の所属フェーズによって、どちら側なのかを選定
	if(pAttack->getPhase()==Phase::PLAYER)
		pBattle->setSide(CDataBattle::RIGHT);
	else
		pBattle->setSide(CDataBattle::LEFT);

	// 戦闘背景設定
	pBattle->setBack(p->getMap()->getDemoBack());

	// カウンター発動判定
	bool bCounter=false;
	if(state_.getWeaponID(CBattleState::COUNTER)>=0)
	{// 反撃するなら判定
		bCounter = IsCounter(*pCounter, *(p->getWeaponData(state_.getWeaponID(CBattleState::COUNTER))), *pAttack, *p);
	}

	// 反撃しない時は、相手の防御行動がBattle::NOになっている
	// 攻撃の計算
	bool bHit;
	if(!bCounter)
	{// 攻撃
		// とりあえず、攻撃はしてみる
		bHit = calcBattle(CBattleState::ATTACK,CBattleState::COUNTER,0,0,*p);

		// 援護防御あり？
		bHit = calcBattleBackDef(0,*p) || bHit;

		// 戦闘後フラグ倒し
		flagDownSpirit(*pAttack, bHit, attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS);

		// 反撃するか？
		if(state_.getWeaponID(CBattleState::COUNTER)>=0
		&& (state_.getCharaData(CBattleState::COUNTER_BACK)!=NULL || !attack.getAttack().IsDeath()))
		{// 反撃する
			// 相手武器が状態変化でEN減少攻撃で当たってたら、減少分を取得
			// かつ援護防御がない
			bHit = calcBattle(CBattleState::COUNTER,
							  CBattleState::ATTACK,
							  counter.getDefence().getEnAbility() + attack.getAttack().getDamageEN(),
							  attack.getAttack().getEN() + attack.getAttack().getEnAbility(),
							  *p);

			// 戦闘後フラグ倒し
			flagDownSpirit(*pCounter, bHit, counter.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS);
		}
	}
	else
	{// カウンター発動時は、反撃側から攻撃が始まる
		counter.getAttack().setAbility(Ability::COUNTER);

		// 反撃計算
		bHit = calcBattle(CBattleState::COUNTER,CBattleState::ATTACK,0,0,*p);

		// 戦闘後フラグ倒し
		flagDownSpirit(*pCounter, bHit, counter.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS);

		if(!counter.getAttack().IsDeath())
		{// 攻撃側が生きていれば
			// 攻撃をしてくる
			bHit= calcBattle(CBattleState::ATTACK,
						     CBattleState::COUNTER,
							 attack.getDefence().getEnAbility() + counter.getAttack().getDamageEN(),
							 counter.getAttack().getEN() + counter.getAttack().getEnAbility(),
							 *p);

			// んで、援護ある？
			bHit = calcBattleBackDef(attack.getDefence().getEnAbility(),*p) || bHit;

			// 戦闘後フラグ倒し
			flagDownSpirit(*pAttack, bHit, attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS);
		}
		else
		{// 撃破されてても、武器は設定しておく
			attack.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(p->getWeaponData(state_.getState(CBattleState::ATTACK).getWeaponID()),false));
		}
	}

	// 援護攻撃はかならず最後
	if(!counter.getAttack().IsDeath()
	&& (state_.getCharaData(CBattleState::COUNTER_BACK)!=NULL || !attack.getAttack().IsDeath())
	&& state_.getCharaData(CBattleState::ATTACK_BACK)!=NULL)
	{// 援護攻撃がある
		// 援護キャラデータ設定
		CDataBattleBase& back = pBattle->getBattleData(CDataBattle::ATTACK_BACK);
		CDataCharaSLG* pBack = state_.getCharaData(CBattleState::ATTACK_BACK);
		// キャラデータの設定
		back.setChara(smart_ptr<CDataCharaSLG>(pBack,false));
		// 戦闘～
		calcBattle(CBattleState::ATTACK_BACK,
					CBattleState::COUNTER,
					0,
					counter.getAttack().getEN() 
					+ counter.getAttack().getEnAbility() 
					+ counter.getDefence().getEnAbility()
					+ attack.getAttack().getDamageEN(),
					*p,
					state_.getCharaData(CBattleState::COUNTER_BACK)!=NULL
					? 0
					: attack.getAttack().getDamage());

		// 戦闘後フラグ倒し
		// 援護攻撃時は魂・熱血は発動しないのでフラグも倒さない
		flagDownSpirit(*pAttack, false, false);
	}

	// 魔力放出による補正のはずし
	if(nRelease_>=0)
	{
		smart_ptr<CDataCharaSLG>& pChara = pBattle->getBattleData(nRelease_).getChara();
		pChara->getBattle().calcTough(release_.getTough());
		smart_ptr<Weapon::CDataWeaponBattle>& pWeapon = pBattle->getBattleData(nRelease_).getAttack().getWeaponData();
		pWeapon->setAttack(pWeapon->getAttack()-release_.getAttack());
	}

	// 最後に攻撃範囲のクリア
	//p->clearAttack();
}

bool CAttack_action::calcBattle(int nAttack, int nDef, int nAttackEN, int nDefEN, CSLGContext& p, int nAtt)
{// とりあえず、渡ってきたStateから、データを展開する
	//CDbg().Out("Battle %d to %d",nAttack,nDef);
	// 攻撃側データ
	const CBattleStateBase&		attackState = state_.getState(nAttack);
	Weapon::CDataWeaponBattle*	pAttackWeapon = p.getWeaponData(attackState.getWeaponID());
	CDataBattleBase*			attackBattle = p.getBattleData()->getBattleDataPtr(nAttack);
	smart_ptr<CDataCharaSLG>&	pAttackChara = attackBattle->getChara();
	// 出現しておく
	pAttackChara->getState().apper(true);

	// 防御側データ
	CBattleStateBase&			defState = state_.getState(nDef);
	CDataBattleBase*			defBattle = p.getBattleData()->getBattleDataPtr(nDef);
	smart_ptr<CDataCharaSLG>&	pDefChara = defBattle->getChara();
	pDefChara->getState().apper(true);

	// 味方対象武器？
	bool bFriend = pAttackWeapon->getKind()==Weapon::Kind::STATUS;

	// とりあえず、攻撃武器の消費ENを設定
	int nEN = pAttackWeapon->getEN();

	// 聖杯連結はとにかく発動する
	if(pAttackChara->getBattle().IsTalent(Ability::CHALICE_CONECT))
	{// 聖杯連結持ち
		// 反撃時は防御ん時に使ったENも考慮しておく
		// 聖杯連結なければ攻撃できるなら攻撃はする
		int nAttr = ((nAttack==CBattleState::COUNTER) ? nAttackEN+nEN : nEN);
		if(p.getApp()->getAbility().enable(*pAttackChara,nAttr,p,Ability::CHALICE_CONECT))
		{// 使えるってさ
			attackBattle->getAttack().setAbility(Ability::CHALICE_CONECT);
			attackBattle->getAttack().setEnAbility(p.getApp()->getAbility().getEN(Ability::CHALICE_CONECT));
		}
	}

	// 攻撃・反撃時
	if(nAttack==CBattleState::ATTACK
	|| nAttack==CBattleState::COUNTER)
	{
		// 反撃時
		if(nAttack==CBattleState::COUNTER)
		{// もし、今まで消費しきたENとこの武器のENを足して、
			// 現在、ENを越えたら、攻撃は自動的にキャンセルされる
			if(pAttackChara->getBattle().getEN()<nAttackEN+nEN)
			{// 攻撃キャンセル
				defBattle->getDefence().setAction(Battle::NO);
				return false;
			}
		}
		// 魔力放出もち？
		if(nRelease_<0 && pAttackChara->getBattle().IsTalent(Ability::MAGICRELEASE))
		{// もってて使えるなら
			Ability::CAbilityDB& db = p.getApp()->getAbility();
			if(!attackBattle->getAttack().IsAbility(Ability::MAGICRELEASE)
			&& db.enable(*pAttackChara.getPointer(),nAttackEN+nEN,p,Ability::MAGICRELEASE))
			{	// データ適用
				nRelease_=nAttack;
				Ability::CAbility_Magicrelease* pRelease = db.getDataCast<Ability::CAbility_Magicrelease>(Ability::MAGICRELEASE);
				pRelease->applyOffset(release_,pAttackWeapon->getAttack(),pAttackChara->getBattle().getTough());
				attackBattle->getAttack().setAbility(Ability::MAGICRELEASE);
				attackBattle->getAttack().calcEnAbility(pRelease->getEN());
				// 補正適用
				pAttackChara->getBattle().calcTough(-release_.getTough());
				pAttackWeapon->setAttack(pAttackWeapon->getAttack()+release_.getAttack());
			}
		}
	}
	// 消費ENとして設定
	attackBattle->getAttack().setEN(nEN);
	// 使用武器として設定
	attackBattle->getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(pAttackWeapon,false));

	if(nAttack==CBattleState::ATTACK_BACK)
	{// 援護攻撃
		// したという証拠を残す
		attackBattle->getDefence().setAction(Battle::HIT);
		// 援護攻撃時の相手の防御行動は、attackBattleのDefenceになる
		defBattle = attackBattle;
	}
	// 命中判定
	bool bHit=true;
	if(IsSpecialHit(nAttack, pAttackChara.getPointer(), pAttackWeapon, attackBattle, nAttackEN+nEN, nDef, defState, pDefChara.getPointer(), p))
	{// 特殊命中判定
		bHit=true;

		if(nDef==CBattleState::COUNTER_BACK) // 援防時は防御扱い
			defState.setWeaponID(Battle::DEFENCE);
	}
	else
	{// 通常の命中判定
		bHit=calcBattleHit(attackBattle,
						   pAttackChara,
						   attackState,
						   defBattle,
						   pDefChara,
						   defState,
						   nDefEN,
						   bFriend,
						   p);
	}
	
	// 命中したかどうかで、計算分岐
	if(bHit)
	{// 当たる
		calcAndSetBattleDamage(nAttack,
							   attackBattle,
							   pAttackChara,
							   pAttackWeapon,
							   attackState,
							   nDef,
							   defBattle,
							   pDefChara,
							   defState,
							   nDefEN,
							   bFriend,
							   nAtt,
							   p);
	}
	else
	{// 当たらない
		// 防御側の行動を回避にして、ダメージを0に
		defBattle->getDefence().setAction(Battle::AVOID);
	}
	// 当たったかどうかを返す
	return bHit;
}

// 武器やAbility効果によって必ず当たる
bool CAttack_action::IsSpecialHit(int nAttack, CDataCharaSLG* pAttackChara, Weapon::CDataWeaponBattle* pAttackWeapon, CDataBattleBase*	attackBattle, int nEN, int nDef, CBattleStateBase& defState, CDataCharaSLG* pDefChara, CSLGContext& p)
{
	// 援護防御
	if(nDef==CBattleState::COUNTER_BACK) return true;

	// 反撃側で、その武器がアヴェスター
	if(nAttack==CBattleState::COUNTER 
	&& (pAttackWeapon->IsWeaponID("AVESTER") || pAttackWeapon->IsWeaponID("AVESTER_ENEMY"))
	)	return true;

	// 攻撃だが反撃されて、それがアヴェスター
	if(nAttack==CBattleState::ATTACK 
	&& defState.getWeaponID()>=0 
	&& (p.getWeaponData(defState.getWeaponID())->IsWeaponID("AVESTER") || p.getWeaponData(defState.getWeaponID())->IsWeaponID("AVESTER_ENEMY"))
	)	return true;

	// 未来視・弐が発動してたら必ず当たる
	// 相手がひらめいてたらその限りでは無い
	if(!pDefChara->getBattle().IsSpirit(Chara::CValidSpirit::AVOID))
	{
		Ability::CAbilityDB& db = p.getApp()->getAbility();
		if(pAttackChara->getBattle().IsTalent(Ability::FUTUREEYE_SECOND) // 未来視・弐
		&& db.enable(*pAttackChara,nEN,p,Ability::FUTUREEYE_SECOND)) 
		{
			attackBattle->getAttack().setAbility(Ability::FUTUREEYE_SECOND);
			attackBattle->getAttack().calcEnAbility(db.getEN(Ability::FUTUREEYE_SECOND));
			return true;
		}
	}

	return false;
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end