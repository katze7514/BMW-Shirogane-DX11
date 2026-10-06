#include "stdafx.h"

#include "../../Chara/ConstChara.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Ability/IDAbility.h"

#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "IDAction.h"
#include "CActionSimple.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionSimple::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::PLAYER;
		s << nID;
		nID=0;
		s << nID;
	}
}

void CActionSimple::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::PLAYER;
}

void CActionSimple::actionPhasePer(SLG::CDataCharaSLG& chara, CSLGContext& context)
{// フェーズ切り替え時毎に呼ばれる
	// 状態変化効果ターンdec
	chara.getBattle().condDec();
}

void CActionSimple::actionPhaseStart(SLG::CDataCharaSLG& chara, CSLGContext& context, bool bIntro, bool bReset)
{
	bool bEN=false; // 固定回復あり？

	// ターンはじめの色々な処理
	Chara::CDataCharaBattle& battle = chara.getBattle();

	// リセットフラグが立ってたらいろいろリセット
	if(bReset)
	{
		// 気力
		battle.setMental(MENTAL_NORMAL);
		// 援護回数
		battle.setBackUpAttack(0);
		battle.setBackUpDefence(0);
	}

	// エースボーナス
	if(bIntro && battle.getKill()>=50)
		battle.calcMental(5);

	// 精神フラグ倒し
	// 必中
	battle.spirit(false,Chara::CValidSpirit::HIT);
	// 集中
	battle.spirit(false,Chara::CValidSpirit::CONCENT);
	// 信念
	battle.spirit(false,Chara::CValidSpirit::FAITH);
	// 鉄壁
	battle.spirit(false,Chara::CValidSpirit::DEFENCE);
	// 突撃
	battle.spirit(false,Chara::CValidSpirit::CHARGE);

	// 技能効果
	// 戦意高揚(登場時は発動しない)
	if(!bIntro && battle.hasSkill(Ability::FIGHTUP)>=0)
		battle.calcMental(2);
	// 闘争心(登場時のみ発動)
	if(bIntro && battle.hasSkill(Ability::BATTLESPIRIT)>=0)
		battle.calcMental(5);

	Ability::CAbilityDB& db = context.getApp()->getAbility();
	// HP回復
	if(battle.IsTalent(Ability::HP_RECOVER_S))
		db.applyStatus(chara,0,Ability::HP_RECOVER_S);
	ef(battle.IsTalent(Ability::HP_RECOVER_M))
		db.applyStatus(chara,0,Ability::HP_RECOVER_M);
	ef(battle.IsTalent(Ability::HP_RECOVER_L))
		db.applyStatus(chara,0,Ability::HP_RECOVER_L);
		
	// EN回復
	if(battle.IsTalent(Ability::EN_RECOVER_S))
		db.applyStatus(chara,0,Ability::EN_RECOVER_S);
	ef(battle.IsTalent(Ability::EN_RECOVER_M))
		db.applyStatus(chara,0,Ability::EN_RECOVER_M);
	ef(battle.IsTalent(Ability::EN_RECOVER_L))
		db.applyStatus(chara,0,Ability::EN_RECOVER_L);
	else
		bEN=true;
	
	// SP回復
	if(battle.hasSkill(Ability::SPRECOVER)!=-1)
		battle.calcSP(-10);

	// 援護回数回復
	int nAttr = battle.hasSkill(Ability::BACKUPATTACK);
	if(nAttr!=-1)
		battle.setBackUpAttack(nAttr);

	nAttr = battle.hasSkill(Ability::BACKUPDEFENCE);
	if(nAttr!=-1)
		battle.setBackUpDefence(nAttr);

	// 固有能力
	// 聖杯連結
	if(battle.IsTalent(Ability::CHALICE_CONECT))
	{
		db.applyStatus(chara,0,Ability::CHALICE_CONECT);
		bEN=false;
	}
	// アヴァロン
	if(battle.IsTalent(Ability::AVALON))
		db.applyStatus(chara,0,Ability::AVALON);
	// 真・分割思考
	if(battle.IsTalent(Ability::TRUE_DIVISION))
		db.applyStatus(chara,0,Ability::TRUE_DIVISION);

	// アイテム効果
	Item::CItemDB& iDB = context.getApp()->getItem();
	// イヤリング
	if(bIntro && battle.IsHasItem(Item::EARRING))
	{
		int n = battle.countHasItem(Item::EARRING);
		for(int i=0; i<n; ++i)
			iDB.applyStatus(chara,0,Item::EARRING);
	}
	// リボン
	if(bIntro && battle.IsHasItem(Item::RIBBON))
	{
		int n = battle.countHasItem(Item::RIBBON);
		for(int i=0; i<n; ++i)
			iDB.applyStatus(chara,0,Item::RIBBON);
	}

	// セイバー
	if(battle.IsHasItem(Item::SABER))
		iDB.applyStatus(chara,0,Item::SABER);
	// キャスター
	if(battle.IsHasItem(Item::CASTER))
	{
		iDB.applyStatus(chara,0,Item::CASTER);
		bEN=false;
	}
	// ギル
	if(battle.IsHasItem(Item::GIRU))
		iDB.use(chara,0,context,Item::GIRU);

	// 固定回復
	// ただし、他にENが回復したら行わない
	if(bEN) battle.calcEN(-5);

	// 地形効果
	// 今、立ってるマップチップを取得
	Map::CMapChip* pMap = context.getMapChip(chara.getIndex());
	if(pMap!=NULL)
	{// 念のためNULLチェック
		// HP
		int nHP = pMap->getMapInfo().getHP();
		chara.calcHP(-(battle.getMaxHP()*nHP/100));
		// 10以下にはならない
		if(chara.getBattle().getHP()<=0)
			chara.getBattle().calcHP(-10);

		// EN
		int nEN = pMap->getMapInfo().getEN();
		battle.calcEN(-(battle.getMaxEN()*nEN/100));
	}
}

int CActionSimple::actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP, bool bBackUp)
{
//#ifdef BMW_DEBUG
//	CDbg().Out("ActionCounter %d %d %d %d",nDist, nRealDist, nHeight, nHP);
//#endif
	// 攻撃不可能・射程外・行動不能だったら何もできない
	if(!chara.getState().IsValid(CCharaState::ATTACK)
	|| nDist<0
	|| chara.getBattle().IsCond(Chara::CValidCond::ACTION))
		return Battle::HIT;

	int nID=Battle::HIT;
	int nWeapon;
	int nAtk=INT_MAX;
	chara.getBattle().beginWeapon();
	while(!chara.getBattle().endWeapon())
	{
		nWeapon	= *chara.getBattle().nextWeapon();
		Weapon::CDataWeaponBattle* pWeapon = context.getWeaponData(nWeapon);

		// 仲間を対象に取る武器での反撃はできない
		// マップ兵器でもダメ
		if(pWeapon->getKind()==Weapon::Kind::STATUS
		|| pWeapon->getKind()==Weapon::Kind::CURE
		|| pWeapon->getKind()==Weapon::Kind::REFILL
		|| pWeapon->IsF()
		// 援護時だったら、合体武器も反撃専用もダメ
		|| (bBackUp 
			&& (pWeapon->getKind()==Weapon::Kind::FIGHT_COLLAB 
				|| pWeapon->getKind()==Weapon::Kind::MAGIC_COLLAB
				|| pWeapon->getSpecial()==Weapon::Special::COUNTER_SPECIAL))
		) continue;


		//#ifdef BMW_DEBUG
		//	CDbg().Out("ActionCounter2 %d %d",nWeapon, pWeapon->enable(chara,false,context,nDist,nRealDist,nHeight));
		//#endif

		if(pWeapon->enable(chara,false,context,nDist,nRealDist,nHeight))
		{// 残HPに応じて選択
			if(nAtk>abs(pWeapon->getAttack()-nHP))
			{// 残HPと攻撃力の差がもっとも小さいやつを選択する
				nAtk = abs(pWeapon->getAttack()-nHP);
				nID = nWeapon;
			}
		}
	}
	
	return nID;
}

void CActionSimple::actionMental(SLG::CDataCharaSLG& chara, int nID)
{
	chara.getBattle().calcMental(Chara::Const::MENTAL_UPDOWN_TABLE[chara.getBattle().getChara()][nID]);
}

void CActionSimple::actionBattleEnd(SLG::CDataCharaSLG& chara, CSLGContext& context)
{
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end