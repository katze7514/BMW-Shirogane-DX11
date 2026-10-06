#include "stdafx.h"

#include "../../Chara/CValidSpirit.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../Ability/IDAbility.h"
#include "../../Ability/Ability/CAbility_Fundpower.h"
#include "../../Ability/Ability/CAbility_Magician.h"
#include "../../Ability/Ability/CAbility_Vampire.h"
#include "../../Ability/Ability/CAbility_Specter.h"
#include "../../Ability/Ability/CAbility_Breakline.h"
#include "../../Ability/Ability/CAbility_Origin.h"
#include "../../Ability/Ability/CAbility_Death.h"
#include "../../Ability/Ability/CAbility_Arrowbless.h"
#include "../../Ability/Ability/CAbility_Attacker.h"
#include "../../Ability/Ability/CAbility_Revenge.h"
#include "../../Ability/Ability/CAbility_Guard.h"
#include "../../Ability/Ability/CAbility_Death_Ex.h"
#include "../../Ability/Ability/CAbility_Death_True.h"
#include "../../Ability/Ability/CAbility_Division.h"
#include "../../Ability/Ability/CAbility_Division_True.h"
#include "../../Ability/Ability/CAbility_Demon.h"
#include "../../Ability/Ability/CAbility_Futou.h"
#include "../../Ability/Ability/CAbility_ChaliceConect.h"

#include "../../Spirit/Spirit/CSpirit_Concent.h"
#include "../../Spirit/Spirit/CSpirit_Fireball.h"
#include "../../Spirit/Spirit/CSpirit_Spirit.h"
#include "../../Spirit/Spirit/CSpirit_Defence.h"
#include "../../Spirit/Spirit/CSpirit_Snipe.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/COffsetWeapon.h"
#include "../Context/COffsetBattle.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CAttack_calc.h"

namespace BMW{
namespace SLG{
namespace Attack{

int CAttack_calc::calcHit(CDataCharaSLG& attack, 
						  const Weapon::CDataWeaponBattle* pAttack, 
						  CDataCharaSLG& def,
						  int nDist,
						  int nHeight,
						  CSLGContext& p,
						  bool bBackUp)
{
	using Chara::CValidSpirit;
	// 防御側が行動不能だったら必中と同じ扱い
	if(def.getBattle().IsCond(Chara::CValidCond::ACTION)) return HIT;
	// 必・閃
	// 閃優先
	// 援護時計算の時は、相手の閃きは、無視して計算を続ける
	if(!bBackUp && def.getBattle().IsSpirit(CValidSpirit::AVOID)) return AVOID;
	if(attack.getBattle().IsSpirit(CValidSpirit::HIT)) return HIT;

	// 基本命中値
	int nHit = attack.getBattle().getHit()/2 + 140 + pAttack->getHit();

	// 技能修正
	nHit += calcSkillHit(attack,def,p);

	// 基本回避値
	int nAvoid = def.getBattle().getAvoid()/2 + def.getBattle().getQuick();

	// 技能修正
	nAvoid += calcSkillAvoid(def,attack,p,pAttack->IsT());

	// 計算
	Map::CMapChip* pDefMap = p.getMapChip(def.getIndex());
	// 高さ修正
	// 射線内の最大高さによって決まる
	//int nHeight = 
	//	p.getMapChip(attack.getIndex())->getMapInfo().getHeight()
	//	- pDefMap->getMapInfo().getHeight();
	// 距離修正
	// 地形効果
	int nLand = 0;
	if(!def.getBattle().IsTalent(Ability::LAND_IGNORE))
		nLand = pDefMap->getMapInfo().getHit();

	// 計算
	int calc = (nHit - nAvoid) + nHeight*3 + (3-nDist)*3 + nLand;

	// 防御側がちびキャラだったら計算命中率が80%
	if(def.getBattle().IsTalent(Ability::LOLI))
		calc=(calc*4)/5;
	// 防御側がでかキャラだったら計算命中率が110%
	if(def.getBattle().IsTalent(Ability::DEKA))
		calc=(calc*6)/5;
	// 命中ダウン状態だったら計算命中率が半分
	if(attack.getBattle().IsCond(Chara::CValidCond::HIT))
		calc=calc/2;
	// 回避ダウン状態だったら計算回避率が半分
	if(def.getBattle().IsCond(Chara::CValidCond::AVOID))
		calc=calc*2;

	return calc<0 ? 0 : calc;
}

int CAttack_calc::calcSkillHit(CDataCharaSLG& attack, CDataCharaSLG& def, CSLGContext& p)
{// Abilityによる命中補正計算
 // 底力・吸血種・魔術師・妖怪・直視の魔眼・真祖・分割思考・真・分割思考・不撓不屈
	COffsetHit hit;
	Chara::CDataCharaBattle& battle = attack.getBattle();
	Ability::CAbilityDB& db = p.getApp()->getAbility();

	// 底力
	int nAttr = battle.hasSkill(Ability::FUNDPOWER);
	// 底力持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Fundpower>(Ability::FUNDPOWER)->applyOffset(hit,nAttr,attack);

	// 吸血種
	nAttr = battle.hasSkill(Ability::VAMPIRE);
	// 吸血種持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Vampire>(Ability::VAMPIRE)->applyOffset(hit,nAttr);

	// 魔術師
	nAttr = battle.hasSkill(Ability::MAGICIAN);
	// 魔術師持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Magician>(Ability::MAGICIAN)->applyOffset(hit,nAttr);

	// 妖怪
	nAttr = battle.hasSkill(Ability::SPECTER);
	// 妖怪が発動
	if(nAttr>0) db.getDataCast<Ability::CAbility_Specter>(Ability::SPECTER)->applyOffset(hit,nAttr);

	// 直視の魔眼
	if(battle.IsTalent(Ability::DEATH))
	{
		if(db.enable(attack,0,p,Ability::DEATH))
			db.getDataCast<Ability::CAbility_Death>(Ability::DEATH)->applyOffset(hit);
	}

	// 真祖
	if(battle.IsTalent(Ability::ORIGIN)) db.getDataCast<Ability::CAbility_Origin>(Ability::ORIGIN)->applyOffset(hit);

	// 分割思考
	if(battle.IsTalent(Ability::DIVISION)) db.getDataCast<Ability::CAbility_Division>(Ability::DIVISION)->applyOffset(hit,attack,def);

	// 真・分割思考
	if(battle.IsTalent(Ability::TRUE_DIVISION)) db.getDataCast<Ability::CAbility_Division_True>(Ability::TRUE_DIVISION)->applyOffset(hit,attack,def);

	// 不撓不屈
	if(battle.IsTalent(Ability::FUTOU))
	{
		if(db.enable(attack,0,p,Ability::FUTOU))
			db.getDataCast<Ability::CAbility_Futou>(Ability::FUTOU)->applyOffset(hit);
	}

	return hit.getHit();
}

int CAttack_calc::calcSkillAvoid(CDataCharaSLG& def, CDataCharaSLG& atk, CSLGContext& p, bool bT)
{// Abilityによる回避補正計算
 // 底力・吸血種・魔術師・妖怪・直視の魔眼・矢よけの加護・分割思考
	COffsetHit avoid;
	Chara::CDataCharaBattle& battle = def.getBattle();
	Ability::CAbilityDB& db = p.getApp()->getAbility();

	// 底力
	int nAttr = battle.hasSkill(Ability::FUNDPOWER);
	// 底力持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Fundpower>(Ability::FUNDPOWER)->applyOffset(avoid,nAttr,def);

	// 吸血種
	nAttr = battle.hasSkill(Ability::VAMPIRE);
	// 吸血種持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Vampire>(Ability::VAMPIRE)->applyOffset(avoid,nAttr);

	// 魔術師
	nAttr = battle.hasSkill(Ability::MAGICIAN);
	// 魔術師持ち
	if(nAttr>0) db.getDataCast<Ability::CAbility_Magician>(Ability::MAGICIAN)->applyOffset(avoid,nAttr);

	// 妖怪
	nAttr = battle.hasSkill(Ability::SPECTER);
	// 妖怪が発動
	if(nAttr>0)	db.getDataCast<Ability::CAbility_Specter>(Ability::SPECTER)->applyOffset(avoid,nAttr);

	// 直視の魔眼
	if(battle.IsTalent(Ability::DEATH))
	{// 発動中なら
		if(db.enable(def,0,p,Ability::DEATH))
			db.getDataCast<Ability::CAbility_Death>(Ability::DEATH)->applyOffset(avoid);
	}

	// 矢よけの加護
	if(bT)
	{// 相手の武器が飛び道具だったら発動するかも
		if(battle.IsTalent(Ability::ARROWBLESS))
			db.getDataCast<Ability::CAbility_Arrowbless>(Ability::ARROWBLESS)->applyOffset(avoid);
	}

	// 分割思考
	if(battle.IsTalent(Ability::DIVISION)) db.getDataCast<Ability::CAbility_Division>(Ability::DIVISION)->applyOffset(avoid,def,atk);

	// 真・分割思考
	if(battle.IsTalent(Ability::TRUE_DIVISION)) db.getDataCast<Ability::CAbility_Division_True>(Ability::TRUE_DIVISION)->applyOffset(avoid,def,atk);

	return avoid.getAvoid();
}

int CAttack_calc::calcOffHit(CDataCharaSLG& attack, CDataCharaSLG& def, CSLGContext& p, bool bBackup)
{// 命中絶対補正計算
	int nHit = 0; 
	COffsetHit hit; // 補正値
	Ability::CAbilityDB& db = p.getApp()->getAbility();
	Spirit::CSpiritDB& sp = p.getApp()->getSpirit();

	// 援護攻撃修正
	nHit = bBackup ? 25 : 0;

	// 技能修正
	// 見切り
	db.getDataCast<Ability::CAbility_Breakline>(Ability::BREAKLINE)->applyOffset(hit);
	// 攻撃側
	int nAttr = attack.getBattle().hasSkill(Ability::BREAKLINE);
	if(nAttr!=-1)
	{// 見切りを持っている
	 // 発動してるか？
		if(db.enable(attack,nAttr,p,Ability::BREAKLINE))
			nHit += hit.getHitOff();
	}
	// 防御側
	nAttr = def.getBattle().hasSkill(Ability::BREAKLINE);
	if(nAttr!=-1)
	{// 見切りを持っている
	 // 発動してるか？
		if(p.getApp()->getAbility().enable(def,nAttr,p,Ability::BREAKLINE))
			nHit -= hit.getAvoid();
	}

	// 固有能力
	// 直死の魔眼系
	// 直死・改
	hit.reset();
	db.getDataCast<Ability::CAbility_Death_Ex>(Ability::DEATH_EX)->applyOffset(hit);
	// 攻撃側
	if(attack.getBattle().IsTalent(Ability::DEATH_EX))
	{
		if(db.enable(attack,0,p,Ability::DEATH_EX))
			nHit += hit.getHitOff();
	}
	// 防御側
	if(def.getBattle().IsTalent(Ability::DEATH_EX))
	{// 直死・改
		if(db.enable(def,0,p,Ability::DEATH_EX))
			nHit -= hit.getAvoid();
	}

	// 真・直死
	hit.reset();
	db.getDataCast<Ability::CAbility_Death_True>(Ability::DEATH_TRUE)->applyOffset(hit);
	// 攻撃側
	if(attack.getBattle().IsTalent(Ability::DEATH_TRUE))
	{
		if(db.enable(attack,0,p,Ability::DEATH_TRUE))
			nHit += hit.getHitOff();
	}
	// 防御側
	if(def.getBattle().IsTalent(Ability::DEATH_TRUE))
	{
		if(db.enable(def,0,p,Ability::DEATH_TRUE))
			nHit -= hit.getAvoid();
	}

	// 気配遮断
	if(def.getBattle().IsTalent(Ability::KEHAI_SHADAN))
	{
		if(def.getPhase()==p.getPhase()) // 自フェイズの時
			hit.calcHitOff(-10);
		else // 他フェイズの時
			hit.calcHitOff(-30);

	}

	// 精神修正
	// 集中
	hit.reset();
	sp.getDataCast<Spirit::CSpirit_Concent>(Spirit::CONCENT)->applyOffset(hit);
	// 攻撃側
	if(attack.getBattle().IsSpirit(Chara::CValidSpirit::CONCENT))
		nHit += hit.getHitOff();

	// 防御側
	if(def.getBattle().IsSpirit(Chara::CValidSpirit::CONCENT))
		nHit -= hit.getAvoid();

	// 狙撃
	if(attack.getBattle().IsSpirit(Chara::CValidSpirit::SNIPE))
	{
		hit.reset();
		sp.getDataCast<Spirit::CSpirit_Snipe>(Spirit::SNIPE)->applyOffset(hit);
		nHit += hit.getHitOff();
	}

	return nHit;
}

// 命中抽選器
katzeSDK::Math::CRandLottery CAttack_calc::randLot_;

int CAttack_calc::calcDamage(CDataCharaSLG& attack,	const Weapon::CDataWeaponBattle* pAttack,
							 CDataCharaSLG& def,	const Weapon::CDataWeaponBattle* pDef, 
							 int nFlag, CSLGContext& p, int nDist)
{// ダメージ計算
	// 技能DB
	Ability::CAbilityDB& db = p.getApp()->getAbility();
	// 精神DB
	Spirit::CSpiritDB& sp = p.getApp()->getSpirit();

	COffsetBattle battle;

	// 武器種別に合わせて腕力/魔力の選択
	int nStatus;
	switch(pAttack->getKind())
	{
	case Weapon::Kind::FIGHT:
	case Weapon::Kind::FIGHT_COLLAB:
	case Weapon::Kind::FIGHT_COND:
		nStatus = attack.getBattle().getStrength();

		// 怪力発動？
		if(attack.getBattle().IsTalent(Ability::SUPERPOWER))
		{// 持ってる
			if(db.enable(attack,0,p,Ability::SUPERPOWER))
			{// 使える！
				nStatus += 30;
			}
		}
	break;

	case Weapon::Kind::MAGIC:
	case Weapon::Kind::MAGIC_COLLAB:
	case Weapon::Kind::MAGIC_COND:
		nStatus = attack.getBattle().getMagic();
	break;

	default: nStatus=-1; break;
	}
	// 基本攻撃力
	int nAttack = pAttack->getAttack();

	// 技能修正
	// 妖怪補正
	int nAttr = attack.getBattle().hasSkill(Ability::SPECTER);
	if(nAttr>0) db.getDataCast<Ability::CAbility_Specter>(Ability::SPECTER)->applyOffset(battle,nAttr);
	
	// 補正値加算
	nAttack+=battle.getAttack();
	
	// 中心距離を外してたら、半減
	if(!pAttack->IsCore(nDist))	nAttack /= 2;

	// 攻撃基本値計算
	nAttack = nAttack * (nStatus + attack.getBattle().getMental()) / 200;

	// 防御力計算
	int nTough = calcTough(def,p);

	// 援護防御で比翼を持っていたら、被援護防御対象も比翼を持ってれば
	// Toughそのを足す
	if(nFlag==DEF_B
	&& def.getBattle().IsTalent(Ability::HIYOKURENRI))
	{// 持ってるらしい
		// 本当は↓こうしたくないなー
		// もしくは全部こうするか、次作り直す時は考えよう
		smart_ptr<CDataCharaSLG>& counter = p.getBattleData()->getBattleData(CDataBattle::COUNTER).getChara();
		if(counter->getBattle().IsTalent(Ability::HIYOKURENRI))
			nTough += calcTough(*counter,p);
	}

	// 武器が妄想心音だったら、Tough半分扱い
	if(pAttack->IsWeaponID("MOUSOU_SHINON"))
		nTough=nTough/2;

	// 真祖による補正はキャラデータ生成時に行っておくこと
	// 基本防御値
	int nDefence = nTough * (def.getBattle().getDefence() + def.getBattle().getMental()) / 200;

	// ダメージ
	int nMapDef = 0;
	if(!attack.getBattle().IsTalent(Ability::LAND_IGNORE))
		nMapDef = p.getMapChip(def.getIndex())->getMapInfo().getDefence();
	// とりあえず、マップ効果補正だけ
	int nDamage = (nAttack - nDefence) * (100-nMapDef) / 100;

	// 技能補正
	// アタッカー
	if(attack.getBattle().hasSkill(Ability::ATTACKER)>=0)
	{// アッタカー持ち
		if(db.enable(attack,0,p,Ability::ATTACKER))
			db.getDataCast<Ability::CAbility_Attacker>(Ability::ATTACKER)->applyOffset(battle,nDamage);
	}
	// 狂化
	if(attack.getBattle().IsTalent(Ability::DEMON))
	{// 狂化持ち
		if(db.enable(attack,0,p,Ability::DEMON))
			db.getDataCast<Ability::CAbility_Demon>(Ability::DEMON)->applyOffset(battle,nDamage);
	}

	// 攻撃側がでかキャラだったらダメージ10%アップ
	if(attack.getBattle().IsTalent(Ability::DEKA))
		nDamage=(nDamage*11)/10;

	// 防御側がでかキャラだったらダメージ10%ダウン
	if(def.getBattle().IsTalent(Ability::DEKA))
		nDamage=(nDamage*9)/10;

	// 反撃計算だったら、リベンジ効果も
	if(nFlag==COUNTER)
	{
		// リベンジ
		if(attack.getBattle().hasSkill(Ability::REVENGE)>=0)
		{// リベンジ持ち
			if(db.enable(attack,0,p,Ability::REVENGE))
				db.getDataCast<Ability::CAbility_Revenge>(Ability::REVENGE)->applyOffset(battle,nDamage);
		}
	}
	// ガード
	if(def.getBattle().hasSkill(Ability::GUARD)>=0)
	{// ガード持ち
		if(db.enable(def,0,p,Ability::GUARD))
			db.getDataCast<Ability::CAbility_Guard>(Ability::GUARD)->applyOffset(battle,nDamage);
	}

	// 不撓不屈
	if(def.getBattle().IsTalent(Ability::FUTOU))
	{// 不撓不屈持ち
		if(db.enable(def,0,p,Ability::FUTOU))
			db.getDataCast<Ability::CAbility_Futou>(Ability::FUTOU)->applyOffset(battle,nDamage);
	}

	// 聖杯連結発動？
	if(p.getBattleData()->getBattleDataPtr(nFlag)->getAttack().IsAbility(Ability::CHALICE_CONECT))
		db.getDataCast<Ability::CAbility_ChaliceConect>(Ability::CHALICE_CONECT)->applyOffset(battle,nDamage);

	// 武器補正
	// フラガで相手の反撃武器攻撃力が4500以上だったら、ダメージ1.5倍
	if(nFlag==COUNTER
	&& pAttack->IsWeaponID("FURAGA")
	&& pDef!=NULL
	&& pDef->getAttack()>=4500)
		battle.calcDamage((nDamage*3)/2);

	// 精神補正
	// 鉄壁
	if(def.getBattle().IsSpirit(Chara::CValidSpirit::DEFENCE))
		sp.getDataCast<Spirit::CSpirit_Defence>(Spirit::DEFENCE)->applyOffset(battle,nDamage);

	if(nFlag!=ATTACK_B)
	{// 援護攻撃時は熱血系は有効にならない
		if(attack.getBattle().IsSpirit(Chara::CValidSpirit::SPIRIT))
		{// 魂
			// ダメージ2.5倍
			sp.getDataCast<Spirit::CSpirit_Spirit>(Spirit::SPIRIT)->applyOffset(battle,nDamage);
			// フィールド武器の時はここではフラグを倒さない
			// 援護防御があると二度通ってくることもあるのでここでは倒さない
			// if(!pAttack->IsF()) attack.getBattle().spirit(false,Chara::CValidSpirit::SPIRIT);
		}
		ef(attack.getBattle().IsSpirit(Chara::CValidSpirit::FIREBALL))
		{// 熱血
			// ダメージ2倍
			sp.getDataCast<Spirit::CSpirit_Fireball>(Spirit::FIREBALL)->applyOffset(battle,nDamage);
			// フィールド武器の時はここではフラグを倒さない
			// 援護防御があると二度通ってくることもあるのでここでは倒さない
			// if(!pAttack->IsF()) attack.getBattle().spirit(false,Chara::CValidSpirit::FIREBALL);
		}
	}

	return nDamage + battle.getDamage();
}

int CAttack_calc::calcTough(CDataCharaSLG& def, CSLGContext& p)
{
	COffsetBattle battle;

	Ability::CAbilityDB& db = p.getApp()->getAbility();

	// 基本防御力
	int nTough = def.getBattle().getTough();
	
	// 吸血種
	int nAttr = def.getBattle().hasSkill(Ability::VAMPIRE);
	if(nAttr>0)	db.getDataCast<Ability::CAbility_Vampire>(Ability::VAMPIRE)->applyOffset(battle,nAttr,def);

	// 底力
	nAttr = def.getBattle().hasSkill(Ability::FUNDPOWER);
	if(nAttr>0)	db.getDataCast<Ability::CAbility_Fundpower>(Ability::FUNDPOWER)->applyOffset(battle,nAttr,def);

	// 補正値を足す
	nTough += battle.getTough();

	// 防御ダウン状態だったら耐久が半分
	if(def.getBattle().IsCond(Chara::CValidCond::DEFENCE))
		nTough=nTough/2;

	return nTough;
}

int CAttack_calc::calcCT(CDataCharaSLG& attack,const Weapon::CDataWeaponBattle* pAttack,
						 CDataCharaSLG& def, CSLGContext& p)
{// CT計算
	int nCT = attack.getBattle().getSkill() - def.getBattle().getSkill() + pAttack->getCT();
	// 技能修正
	nCT += calcSkillCT(attack,p);
	if(attack.getPhase()!=Phase::PLAYER) nCT = nCT>>2;
	return nCT<1 ? 1 : nCT;
}

int CAttack_calc::calcSkillCT(CDataCharaSLG& attack, CSLGContext& p)
{// 技能によるCT補正
	COffsetBattle ct;
	// 底力・吸血種・直死の魔眼・真祖
	Chara::CDataCharaBattle& battle = attack.getBattle();
	Ability::CAbilityDB& db = p.getApp()->getAbility();

	// 底力
	int nAttr = battle.hasSkill(Ability::FUNDPOWER);
	if(nAttr>0) db.getDataCast<Ability::CAbility_Fundpower>(Ability::FUNDPOWER)->applyOffset(ct,nAttr,attack);

	// 吸血種
	nAttr = battle.hasSkill(Ability::VAMPIRE);
	if(nAttr>0) db.getDataCast<Ability::CAbility_Vampire>(Ability::VAMPIRE)->applyOffset(ct,nAttr,attack);
	
	// 直死の魔眼
	if(battle.IsTalent(Ability::DEATH)) 
		db.getDataCast<Ability::CAbility_Death>(Ability::DEATH)->applyOffset(ct);

	// 直死・改
	if(battle.IsTalent(Ability::DEATH_EX)) 
		db.getDataCast<Ability::CAbility_Death_Ex>(Ability::DEATH_EX)->applyOffset(ct);

	// 真・直死
	if(battle.IsTalent(Ability::DEATH_TRUE)) 
		db.getDataCast<Ability::CAbility_Death_True>(Ability::DEATH_TRUE)->applyOffset(ct);

	// 真祖
	if(battle.IsTalent(Ability::ORIGIN))
		db.getDataCast<Ability::CAbility_Origin>(Ability::ORIGIN)->applyOffset(ct);

	return ct.getCT();
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end