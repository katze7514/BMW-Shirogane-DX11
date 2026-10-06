#include "stdafx.h"

#include "../../Chara/CValidSpirit.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../Ability/Ability/CAbility_Madred.h"
#include "../../Ability/Ability/CAbility_Twelvecross.h"
#include "../../Ability/Ability/CAbility_Futsuno.h"
#include "../../Ability/Ability/CAbility_Roaias.h"
#include "../../Ability/Ability/CAbility_Magicbarrier_a.h"
#include "../../Ability/Ability/CAbility_Magicbarrier_b.h"
#include "../../Ability/Ability/CAbility_Magicbarrier_c.h"
#include "../../Ability/Ability/CAbility_MudaiShield.h"
#include "../../Ability/Ability/CAbility_MekaBarriar.h"
#include "../../Ability/Ability/CAbility_ChaliceConect.h"
#include "../../Ability/Ability/CAbility_Colabarriar.h"
#include "../../Ability/Ability/CAbility_MekaBarriarWeak.h"

#include "../../Item/Item/CItem_Avenger.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"

#include "CAttack_calc.h"

#include "CAttack_action.h"

namespace BMW{
namespace SLG{
namespace Attack{
///////////////////////////////////////////////////////////////
// カウンター発動判定
///////////////////////////////////////////////////////////////
bool CAttack_action::IsCounter(CDataCharaSLG& counter, Weapon::CDataWeaponBattle& counterWeapon, CDataCharaSLG& attack, CSLGContext& p)
{
	// 反撃武器によっては確定する
	// 反撃武器がフラガラックだったら確定カウンター
	if(counterWeapon.IsWeaponID("FURAGA")
	|| counterWeapon.IsWeaponID("FURAGA_ENEMY"))
	{	return true;	}
	// アヴェスターだったら絶対発動しない
	ef(counterWeapon.IsWeaponID("AVESTER")
	|| counterWeapon.IsWeaponID("AVESTER_ENEMY"))
	{	return false;	}
	else
	{
		// アヴェのカード持ってる？
		if(counter.getBattle().IsHasItem(Item::AVENGER))
		{// 持ってる！
		 // 発動？
			if(p.getApp()->getItem().enable(counter,0,p,Item::AVENGER)) return true;
		}
		
		// 反撃側がカウンター持ち？
		int nAttr = counter.getBattle().hasSkill(Ability::COUNTER);
		// じゃ、発動？
		if(nAttr>0) return p.getApp()->getAbility().enable(counter,attack,nAttr,p,Ability::COUNTER);
	}

	// ここに来たら発動しない
	return false;
}

///////////////////////////////////////////////////////////////
// 具体命中計算判定
///////////////////////////////////////////////////////////////
bool CAttack_action::calcBattleHit(CDataBattleBase*				attackBattle,
								   smart_ptr<CDataCharaSLG>&	pAttackChara,
								   const CBattleStateBase&		attackState,
								   CDataBattleBase*				defBattle,
								   smart_ptr<CDataCharaSLG>&	pDefChara,
								   const CBattleStateBase&		defState,
								   int							nDefEN,
								   bool							bFriend,
								   CSLGContext&					p)
{
	int nHit;
	bool bHit = true;
	// 味方対象の際は、精神は無視されて必ず当たる
	if(!bFriend)
	{
		if(pDefChara->getBattle().IsSpirit(Chara::CValidSpirit::AVOID))
		{// ひらめき
			pDefChara->getBattle().spirit(false,Chara::CValidSpirit::AVOID);
			nHit=0;
			bHit=false;
		}
		ef(pAttackChara->getBattle().IsSpirit(Chara::CValidSpirit::HIT))
		{//	必中
			nHit=200;
			bHit=true;
		}
		else
		{// 普通の計算
			int nAvoid=1;
			// 防御側が回避を選択してたら、計算回避値を半分にする
			if(defState.getWeaponID()==Battle::AVOID) nAvoid=2;
			nHit = attackState.getHit()/nAvoid + attackState.getOffHit();

			if(nHit<0)		nHit=0;
			ef(nHit>200)	nHit=200;

			// 判定
			bHit=(int)CApp::rand_.Get(100)+1<=nHit;
			//bHit = CAttack_calc::randLot_.lot(static_cast<unsigned int>(nHit));
		}
	}
	else
	{// 味方からのは絶対あたる
		nHit=100;
		bHit=true;
	}
	
	// 命中判定

#ifdef BMW_DEBUG
	CDbg().Out("CalcHit %d %d",bHit,nHit);
#endif

	// 命中が200以上だと、分身系は発動しない
	if(!bFriend && bHit && nHit<200)
	{// 当たるなら、分身だ！
		// 分身系Ability発動判定
		bHit = !calcAlterEgo(*pDefChara.getPointer(),defBattle->getDefence(),nDefEN,p,*pAttackChara.getPointer(),attackBattle->getAttack());
	#ifdef BMW_DEBUG
		CDbg().Out("AlterEgo %d", bHit);
	#endif
	}

	return bHit;
}
	
	
/////////////////////////////////////////////////////////
// 分身系特殊技能判定
/////////////////////////////////////////////////////////
bool CAttack_action::calcAlterEgo(CDataCharaSLG& def, CDataBattleAbility& defData, int nEN, CSLGContext& p, CDataCharaSLG& atk, CDataBattleAbility& atkData)
{
	// 分身されない状態か？

	// 直撃
	if(atk.getBattle().IsSpirit(Chara::CValidSpirit::DIRECT)) return false;

	// 直死・改
	if(atk.getBattle().IsTalent(Ability::DEATH_EX)
	&& p.getApp()->getAbility().enable(atk,def,0,p,Ability::DEATH_EX))
	{// 発動してた
		atkData.setAbility(Ability::DEATH_EX);
		atkData.setEnAbility(p.getApp()->getAbility().getEN(Ability::DEATH_EX));
		return false;
	}

	// 分身系技能判定！！
	#ifdef BMW_DEBUG
		CDbg().Out("AlterEgo Calc！！");
	#endif

	// 未来視
	if(def.getBattle().IsTalent(Ability::FUTUREEYE))
	{// 未来視持ってる
		if(p.getApp()->getAbility().enable(def,nEN,p,Ability::FUTUREEYE))
		{
			defData.setAbility(Ability::FUTUREEYE);
			defData.calcEnAbility(p.getApp()->getAbility().getEN(Ability::FUTUREEYE));
			return true;
		}
	}
	// 分身
	ef(def.getBattle().IsTalent(Ability::ALTER_EGO))
	{// 分身持ってる
		if(p.getApp()->getAbility().enable(def,nEN,p,Ability::ALTER_EGO))
		{
			defData.setAbility(Ability::ALTER_EGO);
			defData.calcEnAbility(p.getApp()->getAbility().getEN(Ability::ALTER_EGO));
			return true;
		}
	}
	ef(def.getBattle().IsTalent(Ability::DEATH_TRUE))
	{// 真・直視の魔眼持ってる
		if(p.getApp()->getAbility().enable(def,atk,nEN,p,Ability::DEATH_TRUE))
		{
			defData.setAbility(Ability::DEATH_TRUE);
			defData.calcEnAbility(p.getApp()->getAbility().getEN(Ability::DEATH_TRUE));
			return true;
		}
	}
	ef(def.getBattle().IsTalent(Ability::OPEN_GET))
	{// オープンゲットを持ってる
		if(p.getApp()->getAbility().enable(def,nEN,p,Ability::OPEN_GET))
		{
			defData.setAbility(Ability::OPEN_GET);
			defData.calcEnAbility(p.getApp()->getAbility().getEN(Ability::OPEN_GET));
			return true;
		}
	}
	ef(def.getBattle().IsTalent(Ability::AVALON))
	{// アヴァロンを持ってる
		if(p.getApp()->getAbility().enable(def,nEN,p,Ability::AVALON))
		{
			defData.setAbility(Ability::AVALON);
			defData.calcEnAbility(p.getApp()->getAbility().getEN(Ability::AVALON));
			return true;
		}
	}
	// 必要に応じて追加
	return false;
}

/////////////////////////////////////////////////////////
// 具体戦闘ダメージ計算
/////////////////////////////////////////////////////////
void CAttack_action::calcAndSetBattleDamage(int							nAttack,
											CDataBattleBase*			attackBattle,
										    smart_ptr<CDataCharaSLG>&	pAttackChara,
											Weapon::CDataWeaponBattle*	pAttackWeapon,
											const CBattleStateBase&		attackState,
											int							nDef,
											CDataBattleBase*			defBattle,
											smart_ptr<CDataCharaSLG>&	pDefChara,
											const CBattleStateBase&		defState,
											int							nDefEN,
											bool						bFriend,
											int							nAtt,
											CSLGContext&				p)
{
	Ability::CAbilityDB& db = p.getApp()->getAbility();
	COffsetBattle battle;
	int nDamage;
	// とりあえず、当たったことにしておく
	defBattle->getDefence().setAction(Battle::HIT);

	if(!bFriend)
	{// 通常攻撃
		if(nAttack==CBattleState::COUNTER
		&&(pAttackWeapon->IsWeaponID("AVESTER") || pAttackWeapon->IsWeaponID("AVESTER_ENEMY"))
		)
		{// アヴェスターだったら、食らったダメージを返す
			if(state_.getState(CBattleState::COUNTER_BACK).getCharaData()!=NULL) // 援防あったら0
				nDamage = 0;
			else
				nDamage = defBattle->getAttack().getDamage();
		}
		ef(nDef!=CBattleState::COUNTER_BACK
		&& pDefChara->getBattle().IsSpirit(Chara::CValidSpirit::TOUGH))
		{// 不屈
			// ダメージを10に
			nDamage=10;
			pDefChara->getBattle().spirit(false,Chara::CValidSpirit::TOUGH);
		}
		else
		{// 不屈無し		
			// ダメージ計算（精神による修正などは、calcDamageの中で行われる）
			// どの計算を行っているかによってFlagを設定
			int nFlag=CAttack_calc::ATTACK;
			if(nAttack==CBattleState::COUNTER)		nFlag=CAttack_calc::COUNTER;
			ef(nAttack==CBattleState::ATTACK_BACK)	nFlag=CAttack_calc::ATTACK_B;

			if(nDef==CBattleState::COUNTER_BACK)	nFlag=CAttack_calc::DEF_B;

			nDamage = CAttack_calc::calcDamage(*pAttackChara, pAttackWeapon,
											   *pDefChara, (defState.getWeaponID()>=0 ? p.getWeaponData(defState.getWeaponID()) : NULL),
											   nFlag, p,
											   nAttack==CBattleState::COUNTER ? nCounterDist_ : nDist_);

			// CTは発生したか？
			bool bFuraga = nAttack==CBattleState::COUNTER && (pAttackWeapon->IsWeaponID("FURAGA"));
			if(bFuraga)
			{// 反撃時FURAGAは必ずクリティカル
				attackBattle->getAttack().ct(true);
			}
			else
			{
				int nCT = CAttack_calc::calcCT(*pAttackChara,pAttackWeapon,*pDefChara,p);
				attackBattle->getAttack().ct((int)CApp::rand_.Get(100)+1<=nCT);
			}

			if(attackBattle->getAttack().IsCT())
			{// CT発生したら、ダメージ1.25倍
				nDamage = (nDamage*125)/100;
			}

			if(nAttack==CBattleState::ATTACK_BACK)
			{//	 援護攻撃だと、ダメージは0.75倍
			 // 但し、連携攻撃を持ってる時は、そのまま
				if(pAttackChara->getBattle().hasSkill(Ability::LINKAGEATTACK)<0)
					nDamage = (nDamage*3)/4;
			}
		}

		// 相手が防御してるか？
		if(defState.getWeaponID()==Battle::DEFENCE)
		{//	してるなら、ダメージ1/2
			defBattle->getDefence().setAction(Battle::DEFENCE);
			nDamage /= 2;
		}
		
		// バリア系特殊技能持ち？
		// ただし、直撃があったり、合体攻撃、アヴェスターの時は発動しない
		int nBarriar = 0;
		if(!pAttackChara->getBattle().IsSpirit(Chara::CValidSpirit::DIRECT)
		&& !(pAttackWeapon->getKind()==Weapon::Kind::FIGHT_COLLAB || pAttackWeapon->getKind()==Weapon::Kind::MAGIC_COLLAB)
		&& !(nAttack==CBattleState::COUNTER && (pAttackWeapon->IsWeaponID("AVESTER") || pAttackWeapon->IsWeaponID("AVESTER_ENEMY")))
		)
		{
			// 攻撃側が直死の魔眼・改持ち？
			bool bD = pAttackChara->getBattle().IsTalent(Ability::DEATH_EX)
					  && p.getApp()->getAbility().enable(*pAttackChara.getPointer(),*pDefChara.getPointer(),0,p,Ability::DEATH_EX);

			nBarriar = calcBarriar(nDamage, *pDefChara.getPointer(), defBattle->getDefence(), nDefEN, pAttackWeapon->IsM(), pAttackWeapon->IsT(), p, bD);

			if(nBarriar!=0 && bD)
			{// バリア発動！　んで直死・改も使える！ バリア貫通じゃ！　おら！
				if(!attackBattle->getAttack().IsAbility(Ability::DEATH_EX))
				{// 発動済みだったら、二重になる
					attackBattle->getAttack().setAbility(Ability::DEATH_EX);
					attackBattle->getAttack().setEnAbility(db.getEN(Ability::DEATH_EX));
				}
				// 軽減ダメージ0
				nBarriar=0;
			}
		}

		if(nBarriar!=0) // バリア発動してたら、防御モーション
			defBattle->getDefence().setAction(Battle::DEFENCE);

		// 最低ダメージは10
		// バリアで0になった場合は0でOK
		if(nDamage<10) nDamage= nBarriar!=0 ? 0 : 10;

		if(pDefChara->getBattle().getHP()<=nDamage+nAtt)
		{// 与えるダメージが、防御側の現在値を越えたら、
			// てかげんされているか？
			bool bEasy=false;
			if(nAttack!=CBattleState::ATTACK_BACK
			&& pAttackChara->getBattle().IsSpirit(Chara::CValidSpirit::EASYON))
			{// 技量を比べる
				bEasy = pAttackChara->getBattle().getSkill() >= pDefChara->getBattle().getSkill();
				
				if(bEasy) // 効果発動可能
					nDamage = pDefChara->getBattle().getHP()-nAtt-10;
			}

			if(!bEasy)
			{// 死亡フラグを立てる
				if(pDefChara->getBattle().IsTalent(Ability::BATTLEFOLLOW)
				&& p.getApp()->getAbility().enable(*pDefChara,*pAttackChara,0,p,Ability::BATTLEFOLLOW))
				{// 戦闘続行もってるなら、発動するか判定
					// 発動した！
					// HP10で生き残る
					nDamage = pDefChara->getBattle().getHP()-nAtt-10;
					defBattle->getDefence().setAbility(Ability::BATTLEFOLLOW);
					defBattle->getDefence().calcEnAbility(p.getApp()->getAbility().getEN(Ability::BATTLEFOLLOW));
				}
				ef(pDefChara->getBattle().IsTalent(Ability::SCAPEGOAT)
				&& p.getApp()->getAbility().enable(*pDefChara,nDefEN,p,Ability::SCAPEGOAT))
				{// 身代わりもってるなら、発動するか判定
					// 発動した！
					// ダメージ無効化
					nDamage = 0;
					defBattle->getDefence().setAbility(Ability::SCAPEGOAT);
					defBattle->getDefence().calcEnAbility(p.getApp()->getAbility().getEN(Ability::SCAPEGOAT));
				}
				else
				{	attackBattle->getAttack().death(true);	}
			}
		}
	}
	else
	{// 能力UPだしね
		nDamage = 0;
	}

	// EN減少攻撃だったら、ここで与えるENを設定。ダメージ0だったらもちろん減らない
	if((pAttackWeapon->getKind()==Weapon::Kind::FIGHT_COND || pAttackWeapon->getKind()==Weapon::Kind::MAGIC_COND)
	&& pAttackWeapon->getCond()==Chara::CValidCond::EN
	&& nDamage>0)
		attackBattle->getAttack().setDamageEN(pAttackWeapon->getCondValue());

	// ダメージ設定
	attackBattle->getAttack().setDamage(nDamage);
}
/////////////////////////////////////////////////////////
// バリア系特殊技能判定
/////////////////////////////////////////////////////////
int CAttack_action::calcBarriar(int& nDamage, CDataCharaSLG& def, CDataBattleAbility& defData, int nEN, bool bM, bool bT, CSLGContext& p, bool bD)
{
	using namespace Ability;
	CAbilityDB& db = p.getApp()->getAbility();
	COffsetBattle battle;
	// 紅赤朱
	if(def.getBattle().IsTalent(Ability::MADRED))
	{// 紅赤朱持ってる！
		// 発動？
		if(db.enable(def,nEN,p,Ability::MADRED))
		{// するよ！
			CAbility_Madred* pRed = db.getDataCast<CAbility_Madred>(Ability::MADRED);
			// なら本発動
			if(!bD)
			{// 泥臭いけど、直死改発動なら、バリアは発動しない
				defData.setAbility(Ability::MADRED);
				defData.calcEnAbility(pRed->getEN());
			}
			// ダメージを減らす
			pRed->applyOffset(battle,nDamage);
		}
	}
	ef(def.getBattle().IsTalent(Ability::TWELVECROSS))
	{// 十二の試練持ってる
		// 発動？
		if(db.enable(def,nEN,p,Ability::TWELVECROSS))
		{// するよ！
			CAbility_Twelvecross* pTwe = db.getDataCast<CAbility_Twelvecross>(Ability::TWELVECROSS);
			if(!bD)
			{// 泥臭いけど、直死改発動なら、バリアは発動しない
				// なら本発動
				defData.setAbility(Ability::TWELVECROSS);
			}
			// ダメージを減らす
			pTwe->applyOffset(battle,nDamage);
		}
	}
	ef(def.getBattle().IsTalent(Ability::FUTSUNO))
	{// フツノバリア持ってる
		// 発動？
		if(db.enable(def,nEN,p,Ability::FUTSUNO))
		{// するよ！
			CAbility_Futsuno* pFutsu = db.getDataCast<CAbility_Futsuno>(Ability::FUTSUNO);
			if(!bD)
			{// 泥臭いけど、直死改発動なら、バリアは発動しない
				// なら本発動
				defData.setAbility(Ability::FUTSUNO);
				defData.calcEnAbility(pFutsu->getEN());
			}
			// ダメージを減らす
			pFutsu->applyOffset(battle,nDamage);
		}
	}
	ef(def.getBattle().IsTalent(Ability::ROAIAS))
	{// ローアイアス持ってる
		// 発動？
		if(db.enable(def,nEN,p,Ability::ROAIAS))
		{// するよ！
			CAbility_Roaias* pRoa = db.getDataCast<CAbility_Roaias>(Ability::ROAIAS);
			if(!bD)
			{// 泥臭いけど、直死改発動なら、バリアは発動しない
				// なら本発動
				defData.setAbility(Ability::ROAIAS);
				defData.calcEnAbility(pRoa->getEN());
			}
			// ダメージを減らす
			pRoa->applyOffset(battle);
			if(bT)
			{// 相手武器がT属性だったら、効果3倍！
				pRoa->applyOffset(battle);
				pRoa->applyOffset(battle);
			}
		}
	}
	ef(def.getBattle().IsTalent(Ability::MUDAI_SHIELD))
	{// ムダイシールド
		// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MUDAI_SHIELD))
			{// するよ！
				CAbility_MudaiShield* pMudai = db.getDataCast<CAbility_MudaiShield>(Ability::MUDAI_SHIELD);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MUDAI_SHIELD);
					defData.calcEnAbility(pMudai->getEN());
				}
				// ダメージを減らす
				pMudai->applyOffset(battle);
			}
	}
	ef(def.getBattle().IsTalent(Ability::MEKA_BARRIAR))
	{// メカバリアー
		// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MEKA_BARRIAR))
			{// するよ！
				CAbility_MekaBarriar* pMeka = db.getDataCast<CAbility_MekaBarriar>(Ability::MEKA_BARRIAR);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MEKA_BARRIAR);
					defData.calcEnAbility(pMeka->getEN());
				}
				// ダメージを減らす
				pMeka->applyOffset(battle);
			}
	}
	ef(def.getBattle().IsTalent(Ability::MEKA_BARRIAR_WEAK))
	{// メカバリアー
		// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MEKA_BARRIAR_WEAK))
			{// するよ！
				CAbility_MekaBarriarWeak* pMeka = db.getDataCast<CAbility_MekaBarriarWeak>(Ability::MEKA_BARRIAR_WEAK);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MEKA_BARRIAR_WEAK);
					defData.calcEnAbility(pMeka->getEN());
				}
				// ダメージを減らす
				pMeka->applyOffset(battle);
			}
	}
	ef(bM)
	{// 相手の攻撃が魔術系だったら、対魔力障壁発動かもよ？
		if(def.getBattle().IsTalent(Ability::MAGICBARRIER_A))
		{// A
			// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MAGICBARRIER_A))
			{// するよ！
				CAbility_Magicbarrier_a* pMagic = db.getDataCast<CAbility_Magicbarrier_a>(Ability::MAGICBARRIER_A);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MAGICBARRIER_A);
					defData.calcEnAbility(pMagic->getEN());
				}
				// ダメージを減らす
				pMagic->applyOffset(battle);
			}
		}
		ef(def.getBattle().IsTalent(Ability::MAGICBARRIER_B))
		{// B
			// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MAGICBARRIER_B))
			{// するよ！
				CAbility_Magicbarrier_b* pMagic = db.getDataCast<CAbility_Magicbarrier_b>(Ability::MAGICBARRIER_B);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MAGICBARRIER_B);
					defData.calcEnAbility(pMagic->getEN());
				}
				// ダメージを減らす
				pMagic->applyOffset(battle);
			}
		}
		ef(def.getBattle().IsTalent(Ability::MAGICBARRIER_C))
		{// C
			// 発動？
			if(p.getApp()->getAbility().enable(def,nEN,p,Ability::MAGICBARRIER_C))
			{// するよ！
				CAbility_Magicbarrier_c* pMagic = db.getDataCast<CAbility_Magicbarrier_c>(Ability::MAGICBARRIER_C);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::MAGICBARRIER_C);
					defData.calcEnAbility(pMagic->getEN());
				}
				// ダメージを減らす
				pMagic->applyOffset(battle);
			}
		}
		ef(def.getBattle().IsTalent(Ability::COLA_BARRIAR))
		{// コーラバリア
			if(db.enable(def,nEN,p,Ability::COLA_BARRIAR))
			{// するよ！
				CAbility_Colabarriar* pCola = db.getDataCast<CAbility_Colabarriar>(Ability::COLA_BARRIAR);
				if(!bD)
				{// 泥臭いけど、直死改発動なら、バリアは発動しない
					// なら本発動
					defData.setAbility(Ability::COLA_BARRIAR);
					defData.calcEnAbility(pCola->getEN());
				}
				// ダメージを減らす
				pCola->applyOffset(battle,nDamage);
			}
		}
	}
	// その他あれば適宜追加

	// 直死が発動してなかったら軽減
	if(!bD)	nDamage += battle.getDamage();

	// 軽減量は、負で入ってるので
	return -battle.getDamage();
}

/////////////////////////////////////////////////////////
// 援護防御計算
/////////////////////////////////////////////////////////
bool CAttack_action::calcBattleBackDef(int nEN, CSLGContext& p)
{
	if(p.getBattleData()->getBattleData(CDataBattle::COUNTER).getDefence().getAction()!=Battle::AVOID
	&& state_.getCharaData(CBattleState::COUNTER_BACK)!=NULL)
	{// 当たって、援護防御あるね
		// 攻撃側の攻撃データクリア
		p.getBattleData()->getBattleData(CDataBattle::ATTACK).getAttack().clearData();
		// 反撃側の防御データクリア
		p.getBattleData()->getBattleData(CDataBattle::COUNTER).getDefence().clearData();

		// 魔力放出データクリア
		//release_.reset();
		//nRelease_ = -1;

		// 援護キャラデータ設定
		CDataBattleBase&	back	= p.getBattleData()->getBattleData(CDataBattle::COUNTER_BACK);
		CDataCharaSLG*		pBack	= state_.getCharaData(CBattleState::COUNTER_BACK);
		// キャラデータの設定
		back.setChara(smart_ptr<CDataCharaSLG>(pBack,false));
		// 攻撃計算
		calcBattle(CBattleState::ATTACK,
					CBattleState::COUNTER_BACK,
					nEN,
					0,
					p);

		return true;
	}

	return false;
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end